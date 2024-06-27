//EVENT_MANAGER???
#include "EventManager.h"


volatile unsigned char Flag_Send_Terminal = 0; //cada 2 segundos
volatile unsigned char Flag_Transmition_Allowed = 0; 
volatile unsigned char Flag_Reception_Detected = 0; 


/*Prototipo funciones privadas
void Update_Terminal();
void Reception_Detected();
void Transmition_Allowed();
//-------------------------------*/

void EventManager_Background(){
	if (Flag_Reception_Detected){	//se detecto una tecla presionada
		Reception_Detected();
		Flag_Reception_Detected = 0;
	}
	if(Flag_Send_Terminal){			//pasaron 2 segundos
		Update_Terminal();
		Flag_Send_Terminal = 0;
	}
	if (Flag_Transmition_Allowed){	//Buffer disponible para empezar a transmitir
		Transmition_Allowed();
		Flag_Transmition_Allowed = 0;
	}
}

//-----------------------BACKGROUND------------------------------

void Update_Terminal(){
	char resultado[BUFFER_SIZE];
	// Variables para almacenar los datos de humedad y temperatura
	char hum[6],temp[6];

	uint8_t year, month, day, hour, minute, second;
	RTC_getDateTime(&year, &month, &day, &hour, &minute, &second);      
	        
	sprintf(resultado, "\nFecha: %02d/%02d/%02d Hora: %02d:%02d:%02d\n\r", day, month, year, hour, minute, second);
	// Leer los valores de humedad y temperatura del DHT11
	DHT11_Read_data(hum, temp);
	
	// Transmitir los valores de humedad y temperatura por UART
	strcat(resultado,"Humedad: ");
	strcat(resultado,hum);
	strcat(resultado,"%\n\r");
	strcat(resultado,"Temperatura: ");
	strcat(resultado,temp);
	strcat(resultado,"C\n\r\r");
	        
	// Transmision por UART
	UART_transmit_string(resultado);
	resultado[0]='\0';
}

// Manjeador de interrupciones de buffer vacio -> cuando interrumpe tiene que mandar mas datos para transmitir (asignar a UDR0)
void Transmition_Allowed(){
	
	//UART_Reset_Index(); //static uint8_t i=0;
	char data = UART_Get_Char_From_Buffer();
	if (data){
		UDR0 = data;
	}
	else{
		//Desactivo las interrupciones de transmision
		UART_Reset_Index();
		SerialPort_TX_Interrupt_Disable();
	}
}

void Reception_Detected(){
	//Manejo de interrupciones de lectura terminada
	char data = SerialPort_Recive_Data(); //Leo caracter desde el registro de datos del UART
	
	if (data=='s' || data=='S'){
		if (UART_Transmition_Status()){
			UART_TX_Disable(); //Si estan activadas, las desactivo
			TIMER_Disable();
		}
		else{
			UART_TX_Enable(); //Si estan desactivadas, las activo
			TIMER_Enable();
		}
	}
}


//-------------------------FOREGROUND-------------------

//Manejo de interrupciones de lectura terminada
ISR(USART_RX_vect){
	Flag_Reception_Detected = 1;
}

ISR (TIMER1_COMPA_vect){ //cada dos segundos
	Flag_Send_Terminal = 1;
}
/* Manjeador de interrupciones de buffer vacio -> cuando interrumpe tiene que mandar mas 
   datos para transmitir (asignar a UDR0)*/
ISR(USART_UDRE_vect){ //Interrupcion de que se puede transmitir en la UART
	Flag_Transmition_Allowed = 1;
}
