#ifndef RTC_H
#define RTC_H
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

//inicializar
void setup();

//transformacion de datos
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);

//verificacion
uint8_t isRtcRunning(void);

//setear y obtener fecha y hora
void setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void getDateTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

#endif // RTC_H
