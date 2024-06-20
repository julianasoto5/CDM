
#define DHT11_PIN PINC0
#include "DHT11.h"


uint8_t data[5];
void DHT11_Start(){
	DDRC |= (1<<DHT11_PIN);				// El bus empieza con un nivel alto
	PORTC &= ~(1<<DHT11_PIN);			// Cambia el nivel del bus a bajo
	_delay_ms(20);						// Tiene que esperar al menos 18ms para que el sensor lo detecte
	PORTC |= (1<<DHT11_PIN);			// Vuelve a subir el nivel del bus a alto - deberia esperar 20-40microsegundos pero calculo q eso lo hace el programa?
}

void DHT11_Response(){
	//VOY A COMENTAR ESTO PORQUE LO DEBERIA HACER EL DHT, NO EL MCU --> DDRC &= ~(1<<DHT11_PIN);			//La se�al de respuesta	del DHT11 empieza en bajo
	while(PINC & (1<<DHT11_PIN));		//La se�al de respuesta del DHT11 empieza en bajo, por lo que espera a que la misma empiece
	while((PINC & (1<<DHT11_PIN))==0);	//Mientras el bus este en nivel bajo, espera -> Mientras se est� transmitiendo la se�al de respuesta espera (aprox 80 microsegundos)
	while(PINC & (1<<DHT11_PIN));		//El DHT sube el nivel y lo mantiene por 80 us, despues empieza la transmision de datos.
}

uint8_t DHT11_Read_byte(){
	uint8_t data = 0;
	for(int i=0;i<8;i++){						//por cada bit recibido
		while((PINC & (1<<DHT11_PIN)) == 0);	//Se mantiene durante 50 us en el while esperando a que termine la se�al de inicio de transmision de bit y el bus queda en ALTO
		_delay_us(29);							//Espera durante 29 microsegundos						
		if(PINC & (1<<DHT11_PIN)){				//Si al salir del delay el bus sigue en alto, quiere decir que es un 1.
			data = ((data<<1) | 1);
		}
		else{									//Si al salir del delay el bus vuelve a estar en BAJO, entonces pasaron los 26-28 microsegundos de ALTO que indicarian un 0
			data = (data<<1);
		}
		while(PINC & (1<<DHT11_PIN));			//Espera a que pase la se�al en ALTO del 1 (estar� aprox 40 microsegundos esperando) -> probar como queda en el IF del 1
	}
	return data;
}

uint8_t DHT11_Read_data(char* hum, char* temp){
	uint8_t checksum = 0;
	DHT11_Start();					//manda se�al de inicio
	DHT11_Response();				//espera se�al de respuesta del sensor
	//Inicio de transmisi�n de datos
	data[0] = DHT11_Read_byte();	//humedad int
	data[1] = DHT11_Read_byte();	//humedad dec
	data[2] = DHT11_Read_byte();	//temp int
	data[3] = DHT11_Read_byte();	//temp dec
	data[4] = DHT11_Read_byte();	//checksum
	//Fin de trama 5*8bits = 40bits
	
	
	DDRC |= 1<<DHT11_PIN;			//Se configura el pin como salida
	PORTC |= 1<<DHT11_PIN;			//Se pone el nivel del bus en ALTO (free)
	
	//chequeo de datos recibidos 
	checksum = data[0] + data[1] + data[2] + data[3];
 
	if (checksum == data[4]){
		sprintf(hum, "%2d.%1d",data[0],data[1]);
		sprintf(temp, "%2d.%1d",data[2],data[3]);
		return 1;
	}
	else{
		return 0;
	}
}