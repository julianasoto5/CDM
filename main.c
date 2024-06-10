#define F_CPU 16000000UL
#define DHT11_PIN PINC0
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "serialPort.h"
#include "dht.h"
#include "RTC.h"

#define BUFFERSIZE 300
 
int main(void) {
    UART_init(0x67); // baud rate calculado para 9600 bps
    DHT11_start();
    rtc_init();
	
	char resultado[BUFFERSIZE];

    // Variables para almacenar los datos de humedad y temperatura
    char hum[6];
    char temp[6];

    uint8_t year, month, day, hour, minute, second;
	sei();
    while (1) {
        rtc_get_datetime(&year, &month, &day, &hour, &minute, &second);
		
        // Formatear la fecha y hora
        char fecha_hora[30];
        sprintf(fecha_hora, "Fecha: %02d/%02d/%02d Hora: %02d:%02d:%02d\n", day, month, year, hour, minute, second);
		
        //UART_transmit_string(fecha_hora);
		strcat(resultado,fecha_hora);
		//UART_transmit_string("\n\r");
		strcat(resultado,"\n\r");
        // Leer los valores de humedad y temperatura del DHT11
        DHT11_read_data(hum, temp);
        // Transmitir los valores de humedad y temperatura por UART
        //UART_transmit_string("Humedad: ");
		strcat(resultado,"Humedad: ");
        //UART_transmit_string(hum);
		strcat(resultado,hum);
        //UART_transmit_string("%\n\r");
		strcat(resultado,"%\n\r");

        //UART_transmit_string("Temperatura: ");
		strcat(resultado,"Temperatura: ");
        //UART_transmit_string(temp);
		strcat(resultado,temp);
        //UART_transmit_string("C\n\r");
		strcat(resultado,"C\n\r");
		
		// Transmision por UART
		UART_transmit_string(resultado);
		resultado[0]='\0';
		
        // Esperar un segundo antes de la próxima lectura
        _delay_ms(2000);
    }

    return 0;
}

