#ifndef __CAMBUS_H__
#define __CAMBUS_H__

#include <stdint.h>
#include "main.h"

#define I2C_TIMEOUT     (1000)

int cambus_scan(I2C_HandleTypeDef *i2c);
int cambus_readb(I2C_HandleTypeDef *i2c, uint8_t reg_addr,  uint8_t *reg_data);
int cambus_writeb(I2C_HandleTypeDef *i2c, uint8_t reg_addr, uint8_t reg_data);

#endif // __CAMBUS_H__
