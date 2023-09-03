#include "configure_SYSCLOCK.h"

void configure_SYSCLOCK() {
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY)) {
  }

  RCC->CFGR |= RCC_CFGR_PLLMULL4;
  RCC->CFGR |= RCC_CFGR_HPRE_DIV2;

  RCC->CFGR |= (1 << RCC_CFGR_PLLSRC_Pos);

  RCC->CR |= RCC_CR_PLLON;
  while(!(RCC->CR & RCC_CR_PLLRDY)) {
  }

  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {
  }

  RCC->CR &= ~(RCC_CR_HSION);

  SystemCoreClockUpdate();
}
