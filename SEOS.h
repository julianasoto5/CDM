#ifndef SEOS_H_
#define SEOS_H_
// -------------------Includes---------------
#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "RTC.h"
#include "DHT11.h"


#define T 1
#define CANT_INT_DHT 2 //CANT_INT * T = Periodo deseado -> 4*0.5s = 2s
#define PREESCALER 256

#define CTC_OCR1A ((F_CPU*T/PREESCALER)-1) 


//---------Prototipo de funciones Publicas-----------------

void SEOS_Scheduler_Tasks();
void SEOS_Dispatch_Tasks();
void SEOS_Init_Timer();
void updateTerminal();


#endif