#include "cambus.h"

uint8_t write_addr = 0x42;
uint8_t read_addr = 0x43;

int cambus_scan(I2C_HandleTypeDef *i2c)
{
    for (uint8_t addr=0x09; addr<=0x77; addr++) {
        if (HAL_I2C_IsDeviceReady(i2c, addr << 1, 10, I2C_TIMEOUT) == HAL_OK) {
            return (addr << 1);
        }
    }

    return 0;
}

int cambus_readb(I2C_HandleTypeDef *i2c, uint8_t reg_addr, uint8_t *reg_data)
{
    int ret = 0;

    if((HAL_I2C_Master_Transmit(i2c, read_addr, &reg_addr, 1, I2C_TIMEOUT) != HAL_OK)
    || (HAL_I2C_Master_Receive (i2c, read_addr, reg_data, 1, I2C_TIMEOUT) != HAL_OK)) {
        ret = 1;
    }
    return ret;
}

int cambus_writeb(I2C_HandleTypeDef *i2c, uint8_t reg_addr, uint8_t reg_data)
{
    int ret = 0;
    uint8_t buf[] = {reg_addr, reg_data};

    HAL_StatusTypeDef resp = HAL_I2C_Master_Transmit(i2c, write_addr, buf, 2, I2C_TIMEOUT);
    if(resp != HAL_OK) {
        ret = 1;
    }
    return ret;
}
