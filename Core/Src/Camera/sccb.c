#include "sccb.h"
#include "delay.h"
 uint32_t ntime;


//void sccb_bus_init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOB,GPIO_Pin_11);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOB,GPIO_Pin_10);
//	SCCB_DATA_OUT;
//
//}




 static void sccb_bus_start(void)
{
    SCCB_SID_H();
    delay_us(I2C_TIM);
    SCCB_SIC_H();
    delay_us(I2C_TIM);
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_L();
    delay_us(I2C_TIM);
}


static void sccb_bus_stop(void)
{
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_H();
    delay_us(I2C_TIM);
    SCCB_SID_H();
    delay_us(I2C_TIM);
}


static void sccb_bus_send_noack(void)
{
	SCCB_SID_H();
	delay_us(I2C_TIM);
	SCCB_SIC_H();
	delay_us(I2C_TIM);
	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_SID_L();
	delay_us(I2C_TIM);
}

static void sccb_bus_send_ack(void)
{
	SCCB_SID_L();
	delay_us(I2C_TIM);
	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_SIC_H();
	delay_us(I2C_TIM);
	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_SID_L();
	delay_us(I2C_TIM);
}

static uint8_t sccb_bus_write_byte(uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++)
	{
		if((data<<i) & 0x80) SCCB_SID_H();
		else 				 SCCB_SID_L();

		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		SCCB_SIC_L();

	}
	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	SCCB_SIC_H();
	delay_us(I2C_TIM);

	tem = !SCCB_SID_STATE;

	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_DATA_OUT;
	return tem;
}

static uint8_t sccb_bus_read_byte(void)
{
	uint32_t i;
	uint8_t read = 0;

	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	for(i = 8; i > 0; i--)
	{
		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		read = read << 1;
		if(SCCB_SID_STATE)
		{
			read += 1;
		}
		SCCB_SIC_L();
		delay_us(I2C_TIM);
	}
    SCCB_DATA_OUT;
	return read;
}

uint8_t wrSensorReg8_8(int regID, int regDat)
{
	delay_us(5);
	sccb_bus_start();
	if(sccb_bus_write_byte(WRITE_ADDR) == 0)
	{
		sccb_bus_stop();
		return 1;
	}
	delay_us(5);
	if(sccb_bus_write_byte(regID) == 0)
	{
		sccb_bus_stop();
		return 2;
	}
	delay_us(5);
	if(sccb_bus_write_byte(regDat)==0)
	{
		sccb_bus_stop();
		return 3;
	}
	sccb_bus_stop();
	return 0;
}

uint8_t rdSensorReg8_8(uint8_t regID, uint8_t* regDat)
{
	delay_us(10);

	sccb_bus_start();
	if(sccb_bus_write_byte(WRITE_ADDR) == 0)
	{
		sccb_bus_stop();
		//goto start;
		return 1;
	}
	delay_us(10);
	if(sccb_bus_write_byte(regID)==0)//ID
	{
		sccb_bus_stop();
		//goto start;
		return 2;
	}
	sccb_bus_stop();
	delay_us(10);
	sccb_bus_start();
	if(sccb_bus_write_byte(WRITE_ADDR|0x01)==0)
	{
		sccb_bus_stop();
		//goto start;
		return 3;
	}
	delay_us(10);
	*regDat = sccb_bus_read_byte();
	sccb_bus_send_noack();
	sccb_bus_stop();
	return 0;
}
