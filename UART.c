
#include "UART.h"
#include "dht.h"
#include "menu.h"
#include "serialPort.h"

#define BUFFER_SIZE 300
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
}

void UART_transmit_char(uint8_t data) {
	// Esperar a que el buffer de transmisión esté vacío
	while (!(UCSR0A & (1 << UDRE0)));
	// Colocar el dato en el registro de datos
	UDR0 = data;
}

void UART_transmit_string(char* str) {
	uint8_t i=0;
	
	SerialPort_TX_Interrupt_Enable();
	while(str[i] != '\0' && i < BUFFER_SIZE){
		BUFFER_TX[i] = str[i];
		i++;
	}
	 //BUFFER_TX[i] = '\0';
}


// Manjeador de interrupciones de buffer vacio -> cuando interrumpe tiene que mandar mas datos para transmitir (asignar a UDR0)
ISR(USART_UDRE_vect){ //Interrupcion de que se puede transmitir en la UART
	//SerialPort_Send_String(*BUFFER_TX);
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
			TX_enabled=0;
		}
		else{
			SerialPort_TX_Enable(); //Si estan desactivadas, las activo
			TX_enabled=1;
		}
	}
}


/*

void SEOS_Schedule_Tasks(void)
{
	if (++cont_hora == 20) { // Cada 1 segundo
		flag_hora = 1;
		cont_hora = 0;
	}
}


void SEOS_Dispatch_Tasks (void) {
	if (flag_hora) {
		// Leer datos del DHT11
		    DHT11_read_data(hum, temp);
			// Transmitir los valores de humedad y temperatura
			
			UART_transmit_string("Humedad: ");
			UART_transmit_string(hum);
			UART_transmit_string("%\n\r");

			UART_transmit_string("Temperatura: ");
			UART_transmit_string(temp);
			UART_transmit_string("C\n\r");

		UART_TX_Interrupt_Enable();
		flag_hora = 0;
	}
}


ISR (TIMER1_COMPA_vect)
{
	SEOS_Schedule_Tasks();
}

uint8_t get_se_apreto_enter(void) //Esta funcion devuelve el valor del flag de si se apreto ENTER
{
	return se_apreto_enter;
}
void set_se_apreto_enter(uint8_t valor) //Esta funcion setea el valor del flag de si se apreto ENTER por el valor pasado en la funcion
{
	se_apreto_enter= valor;
}
*/