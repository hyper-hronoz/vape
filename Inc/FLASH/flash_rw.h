#ifndef __FLASH_ROW__
#define __FLASH_ROW__

#include "stm32f1xx.h"

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

#define VOLTAGE_ADDRESS 0x08007B00

void flash_unlock(void);

void flash_lock();

uint8_t flash_ready(void);

void flash_erase_all_pages(void);

void flash_erase_page(uint32_t address);

void flash_write(uint32_t address, uint32_t data);

#endif
