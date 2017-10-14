#ifndef __UART_H__
#define __UART_H__

#include "stm8l15x.h"
#include "stdio.h"


void uart_config(uint32_t baud_rate);
void uart_send_byte(uint8_t data);

#endif