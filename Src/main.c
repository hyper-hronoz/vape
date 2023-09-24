#include "stm32f1xx.h"

// system clock configurations #include "configure_SYSCLOCK.h"

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

#include "flash_rw.h"

void EXTI4_IRQHandler(void) {
  __disable_irq();

  delay(180);

  if (GPIOC->ODR & 0b1 << 13){
    GPIOC->ODR &= ~(0b1 << 13);
    TIM2->PSC = 720 - 1; 
    TIM2->ARR = 10000 - 1; 
    TIM2->CCER |= 1;      
    TIM2->CCMR1 |= 3 << 5; 
    TIM2->CR1 |= 1;        // запускаем таймер
  } else {
    GPIOC->ODR |= 0b1 << 13;
    TIM2->CR1 &= ~(1);        // отпускаем таймер
    TIM2->CCER &= ~TIM_CCER_CC1E;
  }


  EXTI->PR |= EXTI_PR_PR4;
  __enable_irq();
}

int main() {
  configure_SYSCLOCK(); // ^

  configure_AHB();
  configure_APB1();
  configure_APB2();

  configure_TIM4();
  configure_I2C();

  configure_display();

  uint16_t data = SSD1306.CurrentX;
  uint16_t newData = SSD1306.CurrentY;

  uint32_t currentclock = SystemCoreClock;

  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  // configuring PC13 led
  GPIOC->CRH |= (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1);
  GPIOC->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);
  GPIOC->ODR |= GPIO_ODR_ODR13;

  configure_encoder();

  uint16_t encoder_counter_current = 0;
  uint16_t encoder_counter_prev = 1000;
  int8_t is_read = 0;

  __IO uint32_t saved_encoder_counter_value = *(__IO uint32_t *)VOLTAGE_ADDRESS;
  if (saved_encoder_counter_value < 1000) {
    encoder_counter_current = saved_encoder_counter_value;
  }

  // GPIOA->CRL |= 0b1011; // выход, частота 50МГц
  // GPIOA->CRL &= ~(0b1011); // пушпул, альтернативная функция
  // GPIOA->CRL &= ~(0b1111);
  // GPIOA->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_CNF0;
  GPIOA->CRL &= ~(GPIO_CRL_CNF0_Msk | GPIO_CRL_MODE0_Msk);
  GPIOA->CRL |= (GPIO_CRL_MODE0_Msk);
  GPIOA->CRL |= (GPIO_CRL_CNF0_1);


  // enabling gpio encoder iterrupt
  AFIO->EXTICR[1] &= ~(AFIO_EXTICR2_EXTI4);

  EXTI->IMR |= EXTI_IMR_IM4;
  EXTI->FTSR |= EXTI_FTSR_FT4;

  NVIC_EnableIRQ(EXTI4_IRQn);

  GPIOC->ODR |= 0b1 << 13;

  EXTI->PR |= EXTI_PR_PR4;

  // do not delete without this does not work!!!
  TIM1->CNT |= saved_encoder_counter_value;

  while (1) {
    TIM2->CCR1 = (10000 / 300 - 1) * encoder_counter_current; // на каком числе переключение
    if (is_read) {
      encoder_counter_current = TIM1->CNT;
    }
    if (encoder_counter_current != encoder_counter_prev ||
        encoder_counter_prev == 1000) {
      SSD1306_GotoXY(10, 10);
      char info[100] = {0};
      sprintf(info, "Power: %d   ", encoder_counter_current);
      SSD1306_Puts(info, &Font_7x10, 1);
      SSD1306_UpdateScreen();

      flash_unlock();
      flash_erase_page(VOLTAGE_ADDRESS);
      flash_write(VOLTAGE_ADDRESS, encoder_counter_current);
      flash_lock();

      encoder_counter_prev = encoder_counter_current;
    }
    is_read = 1;
  }
}
