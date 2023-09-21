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

#include "flash_rw.h"

void EXTI4_IRQHandler(void) {
  __disable_irq();
  // NVIC_DisableIRQ(EXTI4_IRQn);
  delay(180);

  if (GPIOC->ODR & 0b1 << 13){
    GPIOC->ODR &= ~(0b1 << 13);
  } else {
    GPIOC->ODR |= 0b1 << 13;
  }


  // NVIC_EnableIRQ(EXTI4_IRQn);
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

  __IO uint32_t saved_encoder_counter_value = *(__IO uint32_t *)0x08007C00;
  if (saved_encoder_counter_value < 1000) {
    encoder_counter_current = saved_encoder_counter_value;
  }


  // GPIOA->CRL |= 0b1011; // выход, частота 50МГц
  // GPIOA->CRL &= 0b1011; // пушпул, альтернативная функция
  
  // TIM2->PSC = 7200 - 1; // насколько делится максимальная частота
  // TIM2->ARR = 10000 - 1; // до скольки таймер считает перед сбросом
  // TIM2->CCR1 = 5000 - 1; // на каком числе переключение
  // TIM2->CCER |= 1;       // разблокируем выход
  // TIM2->CCMR1 |= 3 << 5; // режим ШИМ1
  // TIM2->CR1 |= 1;        // запускаем таймер

  // enabling gpio encoder iterrupt
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->EXTICR[1] &= ~(AFIO_EXTICR2_EXTI4);

  EXTI->IMR |= EXTI_IMR_IM4;
  EXTI->FTSR |= EXTI_FTSR_FT4;

  NVIC_EnableIRQ(EXTI4_IRQn);

  GPIOC->ODR |= 0b1 << 13;

  EXTI->PR |= EXTI_PR_PR4;

  while (1) {
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
      flash_erase_page(VOLATAGE_ADDRESS);
      flash_write(VOLATAGE_ADDRESS, encoder_counter_current);
      flash_lock();

      encoder_counter_prev = encoder_counter_current;
    }
    is_read = 1;
  }
}
