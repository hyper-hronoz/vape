#include "display_transmitter.h"
#include "delay.h"

SSD1306_t SSD1306;
char SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
  /* Set write pointers */
  SSD1306.CurrentX = x;
  SSD1306.CurrentY = y;
}

void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, char *data,
                            uint16_t count) {
  uint8_t dt[256];
  uint8_t i;
  for (i = 0; i < count; i++)
    dt[i] = data[i];
  i2c_WriteMulti(address, reg, data, count);
}

void SSD1306_UpdateScreen(void) {
  uint8_t m;

  for (m = 0; m < 8; m++) {
    SSD1306_WRITECOMMAND(0xB0 + m);
    SSD1306_WRITECOMMAND(0x00);
    SSD1306_WRITECOMMAND(0x10);

    /* Write multi data */
    ssd1306_I2C_WriteMulti(SSD1306_I2C_ADDR, 0x40,
                           &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
  }
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
    /* Error */
    return;
  }

  /* Check if pixels are inverted */
  if (SSD1306.Inverted) {
    color = (SSD1306_COLOR_t)!color;
  }

  /* Set color */
  if (color == SSD1306_COLOR_WHITE) {
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  } else {
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
}

char SSD1306_Putc(char ch, FontDef_t *Font, SSD1306_COLOR_t color) {

  if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
      SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)) {
    return 0;
  }

  for (uint32_t i = 0; i < Font->FontHeight; i++) {
    uint32_t b = Font->data[(ch - 32) * Font->FontHeight + i];
    for (uint32_t j = 0; j < Font->FontWidth; j++) {
      if ((b << j) & 0x8000) {
        SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
                          (SSD1306_COLOR_t)color);
      } else {
        SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
                          (SSD1306_COLOR_t)!color);
      }
    }
  }

  // /* Go through font */
  // for (i = 0; i < Font->FontHeight; i++) {
  // 	b = Font->data[(ch - 32) * Font->FontHeight + i];
  // 	for (j = 0; j < Font->FontWidth; j++) {
  // 		if ((b << j) & 0x8000) {
  // 			SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
  // (SSD1306_COLOR_t) color); 		} else { 			SSD1306_DrawPixel(SSD1306.CurrentX + j,
  // (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
  // 		}
  // 	}
  // }

  SSD1306.CurrentX += Font->FontWidth;

  return ch;
}

char SSD1306_Puts(char *str, FontDef_t *Font, SSD1306_COLOR_t color) {
  /* Write characters */
  while (*str) {
    /* Write character by character */
    if (SSD1306_Putc(*str, Font, color) != *str) {
      /* Return error */
      return *str;
    }

    /* Increase string pointer */
    str++;
  }

  /* Everything OK, zero should be returned */
  return *str;
}

void SSD1306_Fill(SSD1306_COLOR_t color) {
  memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF,
         sizeof(SSD1306_Buffer));
}
