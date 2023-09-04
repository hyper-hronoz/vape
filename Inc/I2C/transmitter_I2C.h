#ifndef __TRANSMITTER_I2C__
#define __TRANSMITTER_I2C__

#include "stm32f1xx.h"

void I2C_write_byte(char saddr, char maddr, char data);

#endif
