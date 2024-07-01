
#include "main.h"

 
int main(void) {
    UART_Init(0x67); // baud rate calculado para 9600 bps
    DHT11_Start();
    I2C_Init();
    RTC_setDateTime(24, 6, 24, 2, 24, 0); // configuración inicial
    TIMER_Init();
    sei();
    while (1) {
		EventManager_Background();
    }
    return 0;
}

