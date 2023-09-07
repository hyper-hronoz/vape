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

// utils
#include "delay.h"

#define SSD1306_I2C_ADDR         0x3C

#define SSD1306_WRITECOMMAND(command)      i2c_writeByte(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data)            i2c_writeByte(SSD1306_I2C_ADDR, 0x40, (data))
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))


void SSD_init() {
	delay(1000);
  SSD1306_WRITECOMMAND(0xAE); //display off
	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); //---set low column address
	SSD1306_WRITECOMMAND(0x10); //---set high column address
	SSD1306_WRITECOMMAND(0x40); //--set start line address
	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
	SSD1306_WRITECOMMAND(0xFF);
	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); //--set normal display
	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_WRITECOMMAND(0x3F); //
	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); //-set display offset
	SSD1306_WRITECOMMAND(0x00); //-not offset
	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); //
	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	SSD1306_WRITECOMMAND(0x12);
	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); //
	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
}

int main() {
  configure_SYSCLOCK(); // ^

  configure_AHB();
  configure_APB1();
  configure_APB2();

  configure_TIM4();
  configure_I2C();

  uint32_t currentclock = SystemCoreClock;

  SSD_init();

  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

  GPIOC->CRH |= (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1);
  GPIOC->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);
  GPIOC->ODR |= GPIO_ODR_ODR13;

  while (1) {
    GPIOC->ODR &= ~(0b1 << 13);
    // delay(1000);
    // GPIOC->ODR |= 0b1 << 13;
    // delay(1000);
  }
}
