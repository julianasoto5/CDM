#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include "lib_delay.h"
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop();
void I2C_Write(uint8_t data);
uint8_t I2C_Read_ack(void);
uint8_t I2C_Read_nack(void);

#endif // I2C_H