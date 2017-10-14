#ifndef __I2C_H__
#define __I2C_H__

#include "tools.h"

void i2c_config(uint32_t clock_frequency, uint16_t own_address);
void i2c_send_data_state(void);

#endif