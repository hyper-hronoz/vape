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

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

void flash_unlock(void) {
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

void flash_lock() { FLASH->CR |= FLASH_CR_LOCK; }

uint8_t flash_ready(void) { return !(FLASH->SR & FLASH_SR_BSY); }

void flash_erase_all_pages(void) {
  FLASH->CR |= FLASH_CR_MER;
  FLASH->CR |= FLASH_CR_STRT;
  while (!flash_ready()) {
  };
  FLASH->CR &= FLASH_CR_MER;
}

void flash_erase_page(uint32_t address) {
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = address;
  FLASH->CR |= FLASH_CR_STRT;
  while (!flash_ready()) {
  }
  FLASH->CR &= ~(FLASH_CR_PER);
}

void flash_write(uint32_t address, uint32_t data) {
  FLASH->CR |= FLASH_CR_PG;
  while (!flash_ready()) {
  }
  *(__IO uint16_t *)address = (uint16_t)data;
  while (!flash_ready()) {
  }
  address += 2;
  data >>= 16;
  *(__IO uint16_t *)address = (uint16_t)data;
  while (!flash_ready()) {
  }
  FLASH->CR &= ~(FLASH_CR_PG);
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

  // configuring PC13 led
  GPIOC->CRH |= (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1);
  GPIOC->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);
  GPIOC->ODR |= GPIO_ODR_ODR13;

  configure_encoder();

  uint16_t encoder_counter_current = 0;
  uint16_t encoder_counter_prev = 0;
  int8_t is_read = 0;

  __IO uint32_t saved_encoder_counter_value = *(__IO uint32_t*)0x08007C00;
  if (saved_encoder_counter_value < 1000) {
    encoder_counter_current = saved_encoder_counter_value;
  }

  TIM1->CNT &= ~(0xFFFFFF);
  TIM1->CNT |= saved_encoder_counter_value;

  while (1) {
    if (is_read) {
      encoder_counter_current = TIM1->CNT;
    }
    if (encoder_counter_current != encoder_counter_prev) {
      SSD1306_GotoXY(10, 10);
      char info[100] = {0};
      sprintf(info, "Power: %d   ", encoder_counter_current);
      SSD1306_Puts(info, &Font_7x10, 1);
      SSD1306_UpdateScreen();

      flash_unlock();
      flash_erase_page(0x08007C00);
      flash_write(0x08007C00, encoder_counter_current);
      flash_lock();

      encoder_counter_prev = encoder_counter_current;
    }
    if (GPIOA->IDR & (0b1 << 4)) {
      GPIOC->ODR &= ~(0b1 << 13);
    } else {
      GPIOC->ODR |= 0b1 << 13;
    }
    is_read = 1;
  }
}
