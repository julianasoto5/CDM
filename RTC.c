
#include "RTC.h"
#include "I2C.h"
#define DS3231_I2C_ADDRESS 0x68

uint8_t decToBcd(uint8_t val) {
    return ( (val/10*16) + (val%10) );
}

uint8_t bcdToDec(uint8_t val) {
    return ( (val/16*10) + (val%16) );
}

void RTC_setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    I2C_Start();
    I2C_Write((DS3231_I2C_ADDRESS << 1) | 0); // Dirección de escritura
    I2C_Write(0); // Comienza en la dirección 0x00
    I2C_Write(decToBcd(second));
    I2C_Write(decToBcd(minute));
    I2C_Write(decToBcd(hour));
    I2C_Write(0); // Día de la semana (no se usa)
    I2C_Write(decToBcd(day));
    I2C_Write(decToBcd(month));
    I2C_Write(decToBcd(year));
    I2C_Stop();
}

void RTC_getDateTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second) {
    I2C_Start();
    I2C_Write((DS3231_I2C_ADDRESS << 1) | 0); // Dirección de escritura
    I2C_Write(0); // Comienza en la dirección 0x00
    I2C_Stop();
    I2C_Start();
    I2C_Write((DS3231_I2C_ADDRESS << 1) | 1); // Dirección de lectura
    *second = bcdToDec(I2C_Read_ack());
    *minute = bcdToDec(I2C_Read_ack());
    *hour = bcdToDec(I2C_Read_ack());
    I2C_Read_ack(); // Día de la semana (no se usa)
    *day = bcdToDec(I2C_Read_ack());
    *month = bcdToDec(I2C_Read_ack());
    *year = bcdToDec(I2C_Read_nack());
    I2C_Stop();
}

void RTC_setup() {
        RTC_setDateTime(24, 6, 24, 2, 24, 0); // Configuración inicial
    
}
