
#include "main.h"

 
int main(void) {
    UART_Init(0x67); // baud rate calculado para 9600 bps
    DHT11_Start();
    I2C_Init();
    RTC_setup();
    TIMER_Init();
    sei();
    while (1) {
		EventManager_Background();
    }
    return 0;
}

