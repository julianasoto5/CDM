#ifndef DHT_H
#define DHT_H
#define DHT11_PIN PINC0
#include <avr/io.h>
#include "lib_delay.h"

uint8_t DHT11_Read_data(char*, char*);
uint8_t DHT11_Read_byte();
void DHT11_Response();
void DHT11_Start();

#endif
