#ifndef __DISPLAY_TRANSMITTER__
#define __DISPLAY_TRANSMITTER__

#include "fonts.h"
#include "stdio.h"
#include "stm32f1xx.h"
#include <string.h>

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 128
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif

#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_WRITEDATA(data) i2c_writeByte(SSD1306_I2C_ADDR, 0x40, (data))
#define SSD1306_WRITECOMMAND(command)                                          \
  i2c_writeByte(SSD1306_I2C_ADDR, 0x00, (command))

#define SSD1306_I2C_ADDR 0x3C

#define SSD1306_WRITECOMMAND(command)                                          \
  i2c_writeByte(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data) i2c_writeByte(SSD1306_I2C_ADDR, 0x40, (data))
/* Absolute value */
#define ABS(x) ((x) > 0 ? (x) : -(x))


void SSD1306_GotoXY(uint16_t x, uint16_t y);

typedef struct {
  uint16_t CurrentX;
  uint16_t CurrentY;
  uint8_t Inverted;
  uint8_t Initialized;
} SSD1306_t;

extern SSD1306_t SSD1306;
extern char SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

char SSD1306_Putc(char ch, FontDef_t *Font, SSD1306_COLOR_t color);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
char SSD1306_Puts(char *str, FontDef_t *Font, SSD1306_COLOR_t color);
void SSD1306_Fill(SSD1306_COLOR_t color);
void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, char *data,
                            uint16_t count);

#endif
