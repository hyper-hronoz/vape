#include "configure_SYSCLOCK.h"
#include "stm32f103xb.h"

void configure_SYSCLOCK() {
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY)) {
  }

  RCC->CR |= (0b1 << RCC_CR_HSEBYP_Pos);

  RCC->CFGR &= ~(RCC_CFGR_PLLXTPRE_HSE_DIV2);

  // switch PLL to HSE
  RCC->CFGR |= (1 << RCC_CFGR_PLLSRC_Pos);

  RCC->CFGR |= RCC_CFGR_PLLMULL4;

  RCC->CR |= RCC_CR_PLLON;
  while(!(RCC->CR & RCC_CR_PLLRDY)) {
  }

  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {
  }

  // ! remove comment in production
  // RCC->CR &= ~(RCC_CR_HSION);

  SystemCoreClockUpdate();
}
