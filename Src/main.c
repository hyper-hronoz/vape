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

#include "configure_encoder.h"

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

  configure_encoder();

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
