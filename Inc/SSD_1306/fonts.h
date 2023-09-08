#ifndef __FONTS__
#define __FONTS__

#include "stm32f1xx.h"


typedef struct {
  uint8_t FontWidth;    /*!< Font width in pixels */
  uint8_t FontHeight;   /*!< Font height in pixels */
  const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

typedef enum {
  SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
  SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

/**
 * @brief  String length and height
 */
typedef struct {
  uint16_t Length; /*!< String length in units of pixels */
  uint16_t Height; /*!< String height in units of pixels */
} FONTS_SIZE_t;

extern FontDef_t Font_7x10;

#endif
