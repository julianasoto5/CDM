#ifndef RTC_H
#define RTC_H

#include <avr/io.h>
#include "lib_delay.h"

//inicializar
void RTC_setup();

//transformacion de datos
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);

//setear y obtener fecha y hora
void RTC_setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void RTC_getDateTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

#endif // RTC_H
