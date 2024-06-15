
#define DHT11_PIN PINC0
#include "DHT11.h"


uint8_t data[5];
void DHT11_Start(){
	DDRC |= (1<<DHT11_PIN);		//configuro como salida
	PORTC &= ~(1<<DHT11_PIN);	// señal baja
	_delay_ms(20);				// tiene que esperar al menos 18ms
	PORTC |= (1<<DHT11_PIN);	/* señal alta*/
}

void DHT11_Response(){
	DDRC &= ~(1<<DHT11_PIN);			//configura como entrada
	while(PINC & (1<<DHT11_PIN));		//mientras no se detecte un nivel bajo se queda esperando
	while((PINC & (1<<DHT11_PIN))==0);	//mientras no se detecte un nivel alto se queda esperando
	while(PINC & (1<<DHT11_PIN));		//mientras no vuelva a detectar un nivel bajo se queda esperando de nuevo
}

uint8_t DHT11_Read_byte(){
	uint8_t data = 0;
	for(int i=0;i<8;i++){
		while((PINC & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);							
		if(PINC & (1<<DHT11_PIN)){  //dht high
			data = ((data<<1) | 1);
		}
		else{
			data = (data<<1);
		}
		while(PINC & (1<<DHT11_PIN));
	}
	return data;
}

uint8_t DHT11_Read_data(char* hum, char* temp){
	uint8_t checksum = 0;
	DHT11_Start();					//señal de inicio
	DHT11_Response();				//
	data[0] = DHT11_Read_byte();	//humedad int
	data[1] = DHT11_Read_byte();	//humedad dec
	data[2] = DHT11_Read_byte();	//temp int
	data[3] = DHT11_Read_byte();	//temp dec
	data[4] = DHT11_Read_byte();	//checksum
	checksum = data[0] + data[1] + data[2] + data[3];
	//end listening
	
	DDRC |= 1<<DHT11_PIN;
	PORTC |= 1<<DHT11_PIN;
	
	if (checksum == data[4]){
		sprintf(hum, "%2d.%1d",data[0],data[1]);
		sprintf(temp, "%2d.%1d",data[2],data[3]);
		return 1;
	}
	else{
		return 0;
	}
}