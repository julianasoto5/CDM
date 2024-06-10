#include "I2C.h"

void i2c_init(void) {
  TWSR = 0x00;
  TWBR = 152;
  TWCR = 0x04;
}

void i2c_start(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
   while ((TWCR & (1 << TWINT)) == 0);
}

void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_write(uint8_t data) {
	TWDR = data;
    TWCR = (1<< TWINT) |(1<<TWEN);
	while ((TWCR & (1 <<TWINT)) == 0);
}

uint8_t i2c_read_ack(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack(void) {
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}