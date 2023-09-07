#ifndef __DISPLAY_TRANSMITTER__
#define __DISPLAY_TRANSMITTER__
#include "stm32f1xx.h"

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 128
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif

static char SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

typedef struct {
  uint16_t CurrentX;
  uint16_t CurrentY;
  uint8_t Inverted;
  uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

typedef enum {
  SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
  SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

typedef struct {
  uint8_t FontWidth;    /*!< Font width in pixels */
  uint8_t FontHeight;   /*!< Font height in pixels */
  const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

/**
 * @brief  String length and height
 */
typedef struct {
  uint16_t Length; /*!< String length in units of pixels */
  uint16_t Height; /*!< String height in units of pixels */
} FONTS_SIZE_t;

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
char SSD1306_Puts(char *str, FontDef_t *Font, SSD1306_COLOR_t color);

#endif
