#include "configure_APB2.h"

void configure_APB2() {
  RCC->CFGR |= RCC_CFGR_PPRE2_2;
}
