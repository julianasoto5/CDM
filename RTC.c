
#include "RTC.h"
#include "I2C.h"
#define DS3231_I2C_ADDRESS 0x68

uint8_t decToBcd(uint8_t val) {
    return ( (val/10*16) + (val%10) );
}

uint8_t bcdToDec(uint8_t val) {
    return ( (val/16*10) + (val%16) );
}

uint8_t isRtcRunning(void) {
    uint8_t status;
    i2c_start();
    i2c_write((DS3231_I2C_ADDRESS << 1) | 0); // Dirección de escritura
    i2c_write(0x0F); // Dirección del registro de estado
    i2c_stop();
    i2c_start();
    i2c_write((DS3231_I2C_ADDRESS << 1) | 1); // Dirección de lectura
    status = i2c_read_nack();
    i2c_stop();

    return !(status & 0x80); // Si el bit OSF (Oscillator Stop Flag) está bajo, el RTC está corriendo
}

void setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    i2c_start();
    i2c_write((DS3231_I2C_ADDRESS << 1) | 0); // Dirección de escritura
    i2c_write(0); // Comienza en la dirección 0x00
    i2c_write(decToBcd(second));
    i2c_write(decToBcd(minute));
    i2c_write(decToBcd(hour));
    i2c_write(0); // Día de la semana (no se usa)
    i2c_write(decToBcd(day));
    i2c_write(decToBcd(month));
    i2c_write(decToBcd(year));
    i2c_stop();
}

void getDateTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second) {
    i2c_start();
    i2c_write((DS3231_I2C_ADDRESS << 1) | 0); // Dirección de escritura
    i2c_write(0); // Comienza en la dirección 0x00
    i2c_stop();
    i2c_start();
    i2c_write((DS3231_I2C_ADDRESS << 1) | 1); // Dirección de lectura
    *second = bcdToDec(i2c_read_ack());
    *minute = bcdToDec(i2c_read_ack());
    *hour = bcdToDec(i2c_read_ack());
    i2c_read_ack(); // Día de la semana (no se usa)
    *day = bcdToDec(i2c_read_ack());
    *month = bcdToDec(i2c_read_ack());
    *year = bcdToDec(i2c_read_nack());
    i2c_stop();
}

void setup() {
    // Verificar si el RTC está corriendo
    if (!isRtcRunning()) {
        // Configurar la fecha y hora iniciales
        setDateTime(24, 6, 9, 22, 7, 0); // Configuración inicial
    }
}
