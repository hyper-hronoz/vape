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

#define SSD1306_I2C_ADDR 0x3C

#define SSD1306_WRITECOMMAND(command)                                          \
  i2c_writeByte(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data) i2c_writeByte(SSD1306_I2C_ADDR, 0x40, (data))
/* Absolute value */
#define ABS(x) ((x) > 0 ? (x) : -(x))

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
  /* Set write pointers */
  SSD1306.CurrentX = x;
  SSD1306.CurrentY = y;
}

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
  
  delay(500);

  SSD1306_GotoXY(10, 30);

  SSD1306_Puts("see", &Font_7x10, 1);

  SSD1306_UpdateScreen(); // display
  
  delay(500);

  uint32_t currentclock = SystemCoreClock;

  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  GPIOC->CRH |= (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1);
  GPIOC->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);
  GPIOC->ODR |= GPIO_ODR_ODR13;

  // GPIOA->CRL &= ~(0b11 << 0); // input mode
  // GPIOA->CRL &= ~(0b11 << 2); // resetting pin value
  // GPIOA->CRL |= (0b11 << 2);

  int counter = 0;

  GPIOB->CRL &= ~(0b11 << 16);
  GPIOA->CRL &= ~(0b11 << 18);
  GPIOA->CRL |= (0b10 << 18);
  GPIOA->ODR |= 0xFFFFFF;

  while (1) {
    // SSD1306_Fill(0);
    // SSD1306_UpdateScreen();
    // delay(1000);
    //
    if (GPIOA->IDR & (0b1 << 4)) {
      GPIOC->ODR &= ~(0b1 << 13);
      char info[100] = {0};
      SSD1306_GotoXY(10, 10);
      sprintf(info, "counter: %d", 0);
      SSD1306_Puts(info, &Font_7x10, 1);
      SSD1306_GotoXY(10, 10);
      SSD1306_UpdateScreen();
      SSD1306_GotoXY(10, 10);
      counter++;
    } else {
      SSD1306_GotoXY(20, 20);
      SSD1306_Puts("fuck", &Font_7x10, 1);
      SSD1306_UpdateScreen();
      GPIOC->ODR |= 0b1 << 13;
    }
  }
}
