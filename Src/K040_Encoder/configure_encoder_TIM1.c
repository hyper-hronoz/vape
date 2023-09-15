#include "configure_encoder_TIM1.h"

void configure_encoder_TIM1() {
  // enabling tim1
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

  // configuaring timer
  TIM1->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
  TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC2S_1);

  // 10: noninverted/rising edge
  TIM1->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  TIM1->CCER &= ~(TIM_CCER_CC2NP | TIM_CCER_CC2NP);

  // 101: Encoder mode 1 - Counter counts up/down on TI2FP1 edge depending on
  // TI1FP2 level
  TIM1->SMCR |= TIM_SMCR_SMS_0;
  TIM1->SMCR &= ~TIM_SMCR_SMS_1;
  TIM1->SMCR &= ~TIM_SMCR_SMS_2;

  // 1111: fSAMPLING = fDTS / 32, N = 8
  TIM1->CCMR1 |= (TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 |
                  TIM_CCMR1_IC1F_3);
  TIM1->CCMR1 |= (TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 |
                  TIM_CCMR1_IC2F_3);

  // 1uto-Reload Register (MAX counter number)
  TIM1->ARR = 300;

  // 1: Counter enabled
  TIM1->CR1 |= TIM_CR1_CEN;
}
