#include "configure_I2C.h"

typedef enum {
	I2C_STANDARD,
	I2C_FAST
} I2C_Mode_Type;


void configure_I2C() {
  uint8_t mode = I2C_FAST;
  uint32_t PCLK1_FREQUENCY = SystemCoreClock / 2;

  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;				// 1: I2C1 clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				// 1: I/O port B clock enabled

	//  this enables afio mapr in short it required if u need select
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;				// 1: Alternate Function I/O clock enabled

	// SCL1
	GPIOB->CRL |= GPIO_CRL_MODE6;					// 11: Output mode, max speed 50 MHz.
	GPIOB->CRL |= GPIO_CRL_CNF6;					// 11: Alternate function output Open-drain
	// SDA1
	GPIOB->CRL |= GPIO_CRL_MODE7;					// 11: Output mode, max speed 50 MHz.
	GPIOB->CRL |= GPIO_CRL_CNF7;					// 11: Alternate function output Open-drain

	// I2C
	I2C1->CR1 |= I2C_CR1_SWRST;						// 1: I2C Peripheral under reset state
	I2C1->CR1 = 0;									// Reset

	I2C1->CR2 |= (PCLK1_FREQUENCY / 1000000);		// FREQ[5:0]: Peripheral clock frequency

	if (mode == I2C_STANDARD) {
		I2C1->CCR = (PCLK1_FREQUENCY / (100000 * 2));					// CCR[11:0]: Clock control register in Fm/Sm mode (Master mode) 100kHz
		I2C1->TRISE = ((PCLK1_FREQUENCY / 1000000) + 1);				// TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode)
	} else if (mode == I2C_FAST) {
		I2C1->CCR = (PCLK1_FREQUENCY / (400000 * 3));					// CCR[11:0]: Clock control register in Fm/Sm mode (Master mode) 400kHz
		I2C1->CCR |= I2C_CCR_FS;										// 1: Fm mode I2C (Fast mode - 400kHz)
		I2C1->TRISE = (((PCLK1_FREQUENCY / 1000000 * 300) / 1000) + 1);	// TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode)
	}

	I2C1->CR1 |= I2C_CR1_PE;						// 1: Peripheral enable
}
