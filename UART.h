#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "serialPort.h"
#define BUFFER_SIZE 300


void UART_init(uint16_t ubrr_value);
void UART_TX_Disable(void);
void UART_TX_Enable(void);
void UART_transmit_string(char* str);

void UART_Reception_Detected();
void UART_Transmition_Allowed();
#endif