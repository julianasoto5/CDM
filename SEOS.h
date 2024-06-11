#ifndef SEOS_H_
#define SEOS_H_
// -------------------Includes---------------
#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "RTC.h"


#define T 0.5
#define PREESCALER 256

#define CTC_OCR1A ((F_CPU*T/PREESCALER)-1) 


//---------Prototipo de funciones Publicas-----------------

void SEOS_Scheduler_Tasks();
void SEOS_Dispatch_Tasks();
void SEOS_Init_Timer();
void updateTerminal();


#endif