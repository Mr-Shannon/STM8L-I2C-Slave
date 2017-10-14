#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "stm8l15x.h"
#include "stm8l15x_conf.h"
#include "stdio.h"
#include "i2c.h"
#include "uart.h"

void system_init(void);
void delay_ms(uint16_t nTime);
void delay_us(uint16_t nTime); 

#endif

