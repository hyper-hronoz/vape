#include "configure_I2C.h"

void configure_I2C() {
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  GPIOB->CRL &= ~(0b11111111 << 24);

  GPIOB->CRL |= ((0b10 << 24) | (0b10 << 28) | 0b11 << 26 | 0b11 << 30); // setting alternate function for PB6, PB7

  I2C1->CR1 &= ~(I2C_CR1_PE);


}
