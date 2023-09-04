#include "transmitter_I2C.h"

// https://blog.embeddedexpert.io/?p=613

void i2c_writeByte(char saddr, char maddr, char data) {
  // wait until bus is not busy
  while (I2C1->SR2 & I2C_SR2_BUSY) {
  }                           

  // generate start bit
  I2C1->CR1 |= I2C_CR1_START; 
  while (!(I2C1->SR1 & I2C_SR1_SB)) {
  } // wait until start bit sent
  
  // sent slave address
  I2C1->DR = saddr << 1;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)) {
  } // wait until address flag is set
    
  // clear SR2 
  volatile uint32_t mr_proper = I2C1->SR2; 
  while (!(I2C1->SR1 & I2C_SR1_TXE)) {
  } // Wait until Data register empty
    
  // sent memory address
  I2C1->DR = maddr; 
  while (!(I2C1->SR1 & I2C_SR1_TXE)) {
  } // wait until data register empty

  I2C1->DR = data;
  while (!(I2C1->SR1 & I2C_SR1_BTF)) {
  } // wait until transfer finished

  // stop bit
  I2C1->CR1 |= I2C_CR1_STOP;
}

