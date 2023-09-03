#include "delay.h"

void delay(uint16_t ms) {
  for (uint16_t i = 0; i < ms; i++) {
    TIM4->SR &= ~(0b1 << 0);

    while(!(TIM4->SR & (0b1 << 0))) {
    }
  }
}
