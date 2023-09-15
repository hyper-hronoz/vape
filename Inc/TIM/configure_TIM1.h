#ifndef __CONFIGURE_TIM1__
#define __CONFIGURE_TIM1__

#include "stm32f1xx.h"

void configure_TIM1() {
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
}

#endif
