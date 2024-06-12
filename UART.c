
#include "UART.h"
#include "DHT11.h"

static volatile char BUFFER_TX [BUFFER_SIZE];
static uint8_t TX_enabled = 0; //desactivado


void UART_init(uint16_t ubrr_value) {
	
	// Configuración de baud rate
	UBRR0H = (uint8_t)(ubrr_value >> 8); // Parte alta del UBRR
	UBRR0L = (uint8_t)ubrr_value;        // Parte baja del UBRR
	// Configuración: 8 bits de datos, sin paridad, 1 bit de parada
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	//SerialPort_Init(ubrr_value);
	SerialPort_TX_Enable();		// Activo el Transmisor del Puerto Serie
	TX_enabled=1;
	SerialPort_RX_Enable();		// Activo el Receptor del Puerto Serie
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion
	// Habilitar transmisión y recepción
	//UCSR0B = (1 << TXEN0) | (1 << RXEN0);
}

void UART_TX_Disable(){
	UCSR0B &=~(1<<TXEN0);
	TX_enabled = 0;
}

void UART_TX_Enable(){
	SerialPort_TX_Enable();
	TX_enabled = 1;
}

void UART_transmit_string(char* str) {
	uint8_t i=0;
	//con un strcpy(BUFFER_TX,str); tambien funciona
	BUFFER_TX[i] = str[i++]; //si entro aca es porque algo para transmitir hay
	SerialPort_TX_Interrupt_Enable();
	while(str[i] != '\0' && i < BUFFER_SIZE){
		BUFFER_TX[i] = str[i];
		i++;
	}
	 BUFFER_TX[i] = '\0';
}


// Manjeador de interrupciones de buffer vacio -> cuando interrumpe tiene que mandar mas datos para transmitir (asignar a UDR0)
ISR(USART_UDRE_vect){ //Interrupcion de que se puede transmitir en la UART
	
	static uint8_t i=0;
	if (BUFFER_TX[i] != '\0'){
		UDR0 = BUFFER_TX[i];
		i++;
		
	}
	else{
		 //Desactivo las interrupciones de transmision
		i=0;
		SerialPort_TX_Interrupt_Disable();
	}
}

//Manejo de interrupciones de lectura terminada
ISR(USART_RX_vect){
	char data = SerialPort_Recive_Data(); //Leo caracter desde el registro de datos del UART
	
	if (data=='s' || data=='S'){
		if (TX_enabled){
			UART_TX_Disable(); //Si estan activadas, las desactivo
		}
		else{
			UART_TX_Enable(); //Si estan desactivadas, las activo
			
		}
	}
}
