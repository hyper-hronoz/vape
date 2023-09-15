#include "stm32f1xx.h"

// system clock configurations
#include "configure_SYSCLOCK.h"

// buses configurations
#include "configure_AHB.h"
#include "configure_APB1.h"
#include "configure_APB2.h"

// interfaces configurations
#include "configure_GPIOB.h"
#include "configure_I2C.h"
#include "configure_TIM4.h"
#include "configure_USART2.h"

#include "transmitter_I2C.h"

#include "configure_display.h"
#include "display_transmitter.h"

// utils
#include "delay.h"

int main() {
  configure_SYSCLOCK(); // ^

  configure_AHB();
  configure_APB1();
  configure_APB2();

  configure_TIM4();
  configure_I2C();

  configure_display();

  SSD1306_GotoXY(10, 30);

  SSD1306_Puts("see", &Font_7x10, 1);

  SSD1306_UpdateScreen(); // display
  //
  uint16_t data = SSD1306.CurrentX;
  uint16_t newData = SSD1306.CurrentY;

  uint32_t currentclock = SystemCoreClock;

  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  // configuring PC13 led
  GPIOC->CRH |= (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1);
  GPIOC->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);
  GPIOC->ODR |= GPIO_ODR_ODR13;

  // configuring encoder button
  GPIOA->CRL &= ~(GPIO_CRL_MODE4_Msk);
  GPIOA->CRL &= ~(GPIO_CRL_CNF4_Msk);
  GPIOA->CRL |= (GPIO_CRL_CNF4_1);
  GPIOA->ODR |= GPIO_ODR_ODR4;

  // enabling tim1
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

  // configuaring timer
  TIM1->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
  TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC2S_1);

  // 10: noninverted/rising edge
  TIM1->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  TIM1->CCER &= ~(TIM_CCER_CC2NP | TIM_CCER_CC2NP);

  // 101: Encoder mode 1 - Counter counts up/down on TI2FP1 edge depending on
  // TI1FP2 level
  TIM1->SMCR |= TIM_SMCR_SMS_0;
  TIM1->SMCR &= ~TIM_SMCR_SMS_1;
  TIM1->SMCR &= ~TIM_SMCR_SMS_2;

  // 1111: fSAMPLING = fDTS / 32, N = 8
  TIM1->CCMR1 |= (TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 |
                  TIM_CCMR1_IC1F_3);
  TIM1->CCMR1 |= (TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 |
                  TIM_CCMR1_IC2F_3);

  // 1uto-Reload Register (MAX counter number)
  TIM1->ARR = 300; 

  // 1: Counter enabled
  TIM1->CR1 |= TIM_CR1_CEN;

  int counter = 0;
  uint16_t Enc_Counter = 0;

  while (1) {
    Enc_Counter = TIM1->CNT;
    if (GPIOA->IDR & (0b1 << 4)) {
      SSD1306_Fill(SSD1306_COLOR_BLACK);
      SSD1306_UpdateScreen();
      GPIOC->ODR &= ~(0b1 << 13);
      char info[100] = {0};
      SSD1306_GotoXY(10, 10);
      // delay(1000);
      sprintf(info, "counter: %d", Enc_Counter);
      SSD1306_Puts(info, &Font_7x10, 1);
      SSD1306_UpdateScreen();
    } else {
      GPIOC->ODR |= 0b1 << 13;
    }
  }
}
