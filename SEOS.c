
#include "SEOS.h"

volatile unsigned char Flag_DHT = 0; //cada 2 segundos

volatile unsigned int contDHT = 0;

void SEOS_Dispatch_Tasks(){
	if(Flag_DHT){
		updateTerminal();
		Flag_DHT = 0;
	}

	
}


ISR (TIMER1_COMPA_vect){ //cada medio segundo
	SEOS_Scheduler_Tasks();
}


void SEOS_Scheduler_Tasks(){
	contDHT++;
	if (contDHT == CANT_INT_DHT){ //si el timer es de 0.5 segundos, a la 4 interrupcion llega a los 2 segundos
		Flag_DHT = 1;
		contDHT = 0;
	}
}


void SEOS_Init_Timer() {				  //Configuracion del Timer de 10Ms 
    TCCR1B |= (1 << WGM12);               // Configura el modo CTC (Clear Timer on Compare Match)
    TCCR1B |= (1 << CS12);				  // Configura el preescalador a 64
    OCR1A = CTC_OCR1A;                    // Establece el valor de comparación
    TIMSK1 |= (1 << OCIE1A);              // Habilita la interrupción de comparación A
}


void updateTerminal(){
		
	char resultado[BUFFER_SIZE];

	// Variables para almacenar los datos de humedad y temperatura
	char hum[6];
	char temp[6];

	uint8_t year, month, day, hour, minute, second;
	RTC_getDateTime(&year, &month, &day, &hour, &minute, &second);
	        
	        
	sprintf(resultado, "Fecha: %02d/%02d/%02d Hora: %02d:%02d:%02d\n\r", day, month, year, hour, minute, second);
	        
			
	// Leer los valores de humedad y temperatura del DHT11
	DHT11_Read_data(hum, temp);
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
	        
}



