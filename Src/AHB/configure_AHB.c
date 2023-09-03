#include "configure_AHB.h"

void configure_AHB() {
  RCC->CFGR |= RCC_CFGR_HPRE_2;
}
