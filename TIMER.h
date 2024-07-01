#ifndef TIMER_H
#define TIMER_H
#define T 2
#define PREESCALER 1024
#define GET_OCR1A ((F_CPU*T/PREESCALER)-1)  //Formula para el OCR1A
#include <avr/io.h>
#include "lib_delay.h"
/*-----------Prototipo funciones publicas-----------*/
void TIMER_Init();
void TIMER_Enable();
void TIMER_Disable();

#endif
