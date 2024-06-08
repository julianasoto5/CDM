#include "rtc.h"
#include <avr/io.h>
#include <util/delay.h>

// Dirección I2C del RTC DS3231
#define DS3231_ADDR 0b1101000

// Función para inicializar la comunicación I2C
void i2c_init() {
    // Configurar los registros TWBR y TWSR para establecer la velocidad de comunicación
    // Por ejemplo, para una frecuencia de reloj de 16MHz y una velocidad de 100kHz:
    TWBR = 72;  // Fórmula: TWBR = ((F_CPU / F_SCL) - 16) / (2 * Prescaler)
    TWSR &= ~(1 << TWPS0) & ~(1 << TWPS1);  // Prescaler = 1 (fórmula de prescaler)
}

// Función para iniciar una transmisión en el bus I2C
void i2c_start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Iniciar la condición de inicio
    while (!(TWCR & (1 << TWINT))); // Esperar a que se complete la condición de inicio
}

// Función para enviar un byte en el bus I2C
void i2c_write(uint8_t data) {
    TWDR = data; // Cargar el dato en el registro de datos
    TWCR = (1 << TWINT) | (1 << TWEN); // Iniciar la transmisión del dato
    while (!(TWCR & (1 << TWINT))); // Esperar a que se complete la transmisión
}

// Función para detener la transmisión en el bus I2C
void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Iniciar la condición de parada
    while ((TWCR & (1 << TWSTO))); // Esperar a que se complete la condición de parada
}

// Función para leer un byte del bus I2C
uint8_t i2c_read(uint8_t ack) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0); // Habilitar la recepción, enviar ACK o NACK
    while (!(TWCR & (1 << TWINT))); // Esperar a que se complete la recepción
    return TWDR; // Leer el dato recibido
}

void rtc_init() {
    // Inicializar la comunicación I2C
    i2c_init();
}

void rtc_get_datetime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second) {
    // Iniciar una transmisión I2C
    i2c_start();
    // Enviar la dirección del RTC DS3231 en modo escritura
    i2c_write(DS3231_ADDR << 1 | 0); // Bit de dirección (R/W = 0)
    // Enviar la dirección del registro de segundos
    i2c_write(0x00);
    // Reiniciar la transmisión I2C
    i2c_start();
    // Enviar la dirección del RTC DS3231 en modo lectura
    i2c_write(DS3231_ADDR << 1 | 1); // Bit de dirección (R/W = 1)
    // Leer los datos del RTC
    *second = i2c_read(1);  // Leer los segundos
    *minute = i2c_read(1);  // Leer los minutos
    *hour = i2c_read(1);    // Leer las horas
    i2c_read(1);            // Leer el día de la semana (no utilizado)
    *day = i2c_read(1);     // Leer el día
    *month = i2c_read(1);   // Leer el mes
    *year = i2c_read(0);    // Leer el año
    // Detener la transmisión I2C
    i2c_stop();
}
