#include "configure_APB1.h"

void configure_APB1() {
  RCC->CFGR |= RCC_CFGR_PPRE1_2;
}
