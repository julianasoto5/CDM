#include "I2C.h"

void I2C_Init(void) {
  TWSR = 0x00;
  TWBR = 152;
  TWCR = 0x04;
}

void I2C_Start(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
   while ((TWCR & (1 << TWINT)) == 0);
}

void I2C_Stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void I2C_Write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) |(1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
}

uint8_t I2C_Read_ack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

uint8_t I2C_Read_nack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}
