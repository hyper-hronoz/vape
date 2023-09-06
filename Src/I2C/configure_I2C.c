#include "configure_I2C.h"

void configure_I2C() {
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  // enable i2c1 clock

  GPIOB->CRL &= ~(0b11111111 << 24);

  GPIOB->CRL = (0b11<< GPIO_CRL_MODE6_Pos | 0b11 << GPIO_CRL_CNF6_Pos);
  GPIOB->CRL = (0b11 << GPIO_CRL_MODE7_Pos | 0b11 << GPIO_CRL_CNF7_Pos);

  AFIO->MAPR = AFIO_MAPR_I2C1_REMAP;

  I2C1->CR1 &= ~(I2C_CR1_PE);

  I2C1->CR1 = I2C_CR1_SWRST;
  I2C1->CR1 &= ~I2C_CR1_SWRST;
  I2C1->CR2 |= 50;
  I2C1->CCR |= 0x2 | (1 << 15) | (1 << 14);
  I2C1->TRISE = 20; // output max rise
  I2C1->CR1 |= I2C_CR1_PE;
}
