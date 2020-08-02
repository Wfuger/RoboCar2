#ifndef __SCCB_H__
#define __SCCB_H__

#include "main.h"

#define I2C_TIM 1

#define WRITE_ADDR 			  0x42
#define READ_ADDR  			  0x43

#define SCCB_SID_PIN          (GPIO_PIN_9)
#define SCCB_SID_PORT         (GPIOB)

#define SCCB_SIC_PIN          (GPIO_PIN_6)
#define SCCB_SIC_PORT         (GPIOB)

#define SCCB_SID_L()       	  HAL_GPIO_WritePin(SCCB_SID_PORT, SCCB_SID_PIN, GPIO_PIN_RESET)
#define SCCB_SID_H()       	  HAL_GPIO_WritePin(SCCB_SID_PORT, SCCB_SID_PIN, GPIO_PIN_SET)

#define SCCB_SIC_L()          HAL_GPIO_WritePin(SCCB_SIC_PORT, SCCB_SIC_PIN, GPIO_PIN_RESET)
#define SCCB_SIC_H()          HAL_GPIO_WritePin(SCCB_SIC_PORT , SCCB_SIC_PIN, GPIO_PIN_SET)



#define SCCB_DATA_IN    	  {GPIOB->MODER |= ~(0x3<<18);}
#define SCCB_DATA_OUT     	  {GPIOB->MODER |= 0x1<<18;}
#define SCCB_SID_STATE	  	  HAL_GPIO_ReadPin(SCCB_SID_PORT, SCCB_SID_PIN)

//void sccb_bus_init(void);
//void sccb_bus_start(void);
//void sccb_bus_stop(void);
//void sccb_bus_send_noack(void);
//void sccb_bus_send_ack(void);
//uint8_t sccb_bus_write_byte(uint8_t data);
//uint8_t sccb_bus_read_byte(void);

uint8_t wrSensorReg8_8(int regID, int regDat);
uint8_t rdSensorReg8_8(uint8_t regID, uint8_t* regDat);


#endif // __SCCB_H__
