#ifndef RTC_H
#define RTC_H

#include <stdint.h>

// Función de inicialización del RTC
void rtc_init();

// Función para obtener la fecha y hora del RTC
void rtc_get_datetime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

#endif // RTC_H