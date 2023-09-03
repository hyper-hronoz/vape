#include "configure_TIM4.h"

void configure_TIM4() {
  // i presume here will be 32MHz

  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

  TIM4->PSC = 31;
  TIM4->ARR = 999;

  TIM4->CR1 |= TIM_CR1_CEN;
}


