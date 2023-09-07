#ifndef __CONFIGURE_DISPLAY__
#define __CONFIGURE_DISPLAY__

#include "stm32f1xx.h"
#include "transmitter_I2C.h"

#define SSD1306_I2C_ADDR         0x3C
#define SSD1306_WRITEDATA(data)            i2c_writeByte(SSD1306_I2C_ADDR, 0x40, (data))
#define SSD1306_WRITECOMMAND(command)      i2c_writeByte(SSD1306_I2C_ADDR, 0x00, (command))

void configure_display(void);

#endif
