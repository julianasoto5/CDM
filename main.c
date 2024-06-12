
#include "main.h"

 
int main(void) {
    UART_init(0x67); // baud rate calculado para 9600 bps
    DHT11_Start();
    I2C_Init();
    //RTC_setup();
	SEOS_Init_Timer();
	
	sei();
    while (1) {
		SEOS_Dispatch_Tasks();
    }
    return 0;
}

