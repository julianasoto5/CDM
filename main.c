#define F_CPU 16000000UL
#define DHT11_PIN PINC0
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "dht.h"
#include "RTC.h"
 
int main(void) {
    UART_init(0x67); // baud rate calculado para 9600 bps
    DHT11_start();
    rtc_init();

    // Variables para almacenar los datos de humedad y temperatura
    char hum[6];
    char temp[6];

    uint8_t year, month, day, hour, minute, second;
    while (1) {
        rtc_get_datetime(&year, &month, &day, &hour, &minute, &second);
		
        // Formatear la fecha y hora
        char fecha_hora[30];
        sprintf(fecha_hora, "Fecha: %02d/%02d/%02d Hora: %02d:%02d:%02d\n", day, month, year, hour, minute, second);
        // Transmitir la fecha y hora por UART
        UART_transmit_string(fecha_hora);
        
		UART_transmit_string("\n\r");
        // Leer los valores de humedad y temperatura del DHT11
        DHT11_read_data(hum, temp);
        // Transmitir los valores de humedad y temperatura por UART
        UART_transmit_string("Humedad: ");
        UART_transmit_string(hum);
        UART_transmit_string("%\n\r");

        UART_transmit_string("Temperatura: ");
        UART_transmit_string(temp);
        UART_transmit_string("C\n\r");

        // Esperar un segundo antes de la próxima lectura
        _delay_ms(1000);
    }

    return 0;
}

