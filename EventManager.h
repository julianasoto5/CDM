#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_
// -------------------Includes---------------
#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "RTC.h"
#include "DHT11.h"


//---------Prototipo de funciones Publicas-----------------

void SEOS_Scheduler_Tasks();
void SEOS_Init_Timer();


#endif