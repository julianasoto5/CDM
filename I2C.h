#ifndef I2C_H
#define I2C_H
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void i2c_init(void);
void i2c_start(void);
void i2c_stop();
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

#endif // I2C_H