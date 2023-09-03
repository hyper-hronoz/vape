#include "stm32f1xx.h"
#include "configure_USART2.h"
#include "configure_I2C.h"
#include "configure_SYSCLOCK.h"
#include "configure_GPIOB.h"
#include "configure_TIM4.h"

int main() {
  // configure_SYSCLOCK();
  uint32_t current_clock = SystemCoreClock;

  while(1) {

  }
}
