
#include "UART.h"
#include "dht.h"
#include "menu.h"

static volatile uint8_t BUFFER_RX[RX_BUFFER_LENGHT]; //Buffer para la recepcion
static volatile uint8_t BUFFER_TX[TX_BUFFER_LENGHT]; //Buffer para la transmision
static volatile uint8_t indiceTX_escritura= 0; //Indice de escritura para el buffer de transmision
static volatile uint8_t indiceTX_lectura= 0; //Indice de lectura para el buffer de transmision
static volatile uint8_t se_apreto_enter= 0; //Flag de si el usuario apreto ENTER
volatile unsigned char flag_hora = 0;
static unsigned char cont_hora = 0;
char hum [5];
char temp [5];


void UART_init(uint16_t ubrr_value) {
	// Configuración de baud rate
	UBRR0H = (uint8_t)(ubrr_value >> 8); // Parte alta del UBRR
	UBRR0L = (uint8_t)ubrr_value;        // Parte baja del UBRR
	// Habilitar transmisión y recepción
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	// Configuración: 8 bits de datos, sin paridad, 1 bit de parada
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit_char(uint8_t data) {
	// Esperar a que el buffer de transmisión esté vacío
	while (!(UCSR0A & (1 << UDRE0)));
	// Colocar el dato en el registro de datos
	UDR0 = data;
}

void UART_transmit_string(const char* str) {
	while (*str) {
		UART_transmit_char(*str++);
	}
}

/*

void UART_TX_Interrupt_Enable(void) //Funcion que activa las interrupciones de transmision de la UART
{
	UCSR0B |= (1<<UDRIE0);
}

void UART_TX_Interrupt_Disable(void) //Funcion que desactiva las interrupciones de transmision de la UART
{
	UCSR0B &=~(1<<UDRIE0);
}

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

ISR(USART_RX_vect){
	static uint8_t i = 0;
	BUFFER_RX[i] = UDR0;
	if (BUFFER_RX[i]=='r') //Si el valor guardado es un ENTER
	{
		BUFFER_RX[i]= '\0'; //Remplaza el valor por un vacio '\0'
		se_apreto_enter= 1; //Activa el flag de que se apreto ENTER
		i=0; //Resetea la variable de iteracion
	}else
	{		if (i==(RX_BUFFER_LENGHT-1)) //Si i llego a la posicion maxima del buffer
		{
			i=0; //Resetea el valor
			BUFFER_RX[i]= 'a';
		}
		i++;
	}
}

ISR(USART_UDRE_vect) //Interrupcion de que se puede transmitir en la UART
{
	if (indiceTX_lectura!=indiceTX_escritura) //Si tengo datos para transmitir
	{
		UDR0= BUFFER_TX[indiceTX_lectura]; //Transmito un caracter del buffer
		indiceTX_lectura= (indiceTX_lectura+1)%(TX_BUFFER_LENGHT-1);
		//Incremento en uno el valor del indice de lectura o lo reseteo si llego al limite
	}else //Si no hay mas nada para transmitir
	{
		UART_TX_Interrupt_Disable(); //Desactivo las interrupciones de transmision
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