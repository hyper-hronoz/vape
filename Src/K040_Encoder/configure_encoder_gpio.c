#include "configure_encoder_gpio.h"

void configure_encoder_gpio() {
  // configuring encoder button
  GPIOA->CRL &= ~(GPIO_CRL_MODE4_Msk);
  GPIOA->CRL &= ~(GPIO_CRL_CNF4_Msk);
  GPIOA->CRL |= (GPIO_CRL_CNF4_1);
  GPIOA->ODR |= GPIO_ODR_ODR4;
}
