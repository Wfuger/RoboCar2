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


#define hdsta 3
#define	susto 3
#define	susta 4
#define	sudat 1
#define	dvdat 3
#define	dvack 3
#define	high 3
#define	low 4
#define	buff 4

#define stretchTime_us 1000

GPIO_InitTypeDef GPIO_InitStruct = {0};

//bool sccb_init = false;
//
//
//void init_sccb()
//{
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_InitStruct.Pin = SCCB_SID_Pin;
//}

void data_in()
{
	LL_GPIO_SetPinMode(SCCB_SID_GPIO_Port, SCCB_SID_Pin, GPIO_MODE_INPUT);
}

void data_out()
{
	LL_GPIO_SetPinMode(SCCB_SID_GPIO_Port, SCCB_SID_Pin, GPIO_MODE_OUTPUT_OD);
}

bool read_sda()
{
	return HAL_GPIO_ReadPin(SCCB_SID_GPIO_Port, SCCB_SID_Pin) == GPIO_PIN_SET;
}

bool read_scl()
{
	return HAL_GPIO_ReadPin(SCCB_SIC_GPIO_Port, SCCB_SIC_Pin) == GPIO_PIN_SET;
}

void clock_stretching(uint32_t t_us)
{
  while ( (read_scl() == 0) && (t_us > 0) )
  {
	delay_us(1);
	t_us--;
  }
}

 void init()
 {
	 SCCB_SID_H();
	 SCCB_SIC_H();
	 delay_us(100);
 }

 void start(void)
 {
   // SCL is high, set SDA from 1 to 0.
   SCCB_SID_L();
   delay_us(hdsta);
   SCCB_SIC_L();
   delay_us(low);
 }

 void stop(void)
 {
   // set SDA to 0
   SCCB_SID_L();

   delay_us(susto);

   SCCB_SIC_H();

   clock_stretching(stretchTime_us);

   // Stop bit setup time, minimum 4us
   delay_us(susto);

   // SCL is high, set SDA from 0 to 1
   SCCB_SID_H();
   delay_us(buff);
 }

 void write_bit(bool bit)
 {

   if (bit)  SCCB_SID_H();
   else		 SCCB_SID_L();

   // SDA change propagation delay
   delay_us(sudat);

   // Set SCL high to indicate a new valid SDA value is available
   SCCB_SIC_H();

   // Wait for SDA value to be read by slave, minimum of 4us for standard mode
   delay_us(high);

   clock_stretching(stretchTime_us);

   // Clear the SCL to low in preparation for next change
   SCCB_SIC_L();
   delay_us(low);
 }

 // Read a bit from I2C bus
 bool read_bit(void) {
   bool bit;

   // Let the slave drive data
//   SCCB_SID_H();
//   data_in();
   // Wait for SDA value to be written by slave, minimum of 4us for standard mode
   delay_us(dvdat);

   // Set SCL high to indicate a new valid SDA value is available
   SCCB_SIC_H();
   clock_stretching(stretchTime_us);

   // Wait for SDA value to be written by slave, minimum of 4us for standard mode
   delay_us(high);

   // SCL is high, read out bit
   bit = read_sda();

   // Set SCL low in preparation for next operation
   SCCB_SIC_L();
//   data_out();

   delay_us(low);

   return bit;
 }

 // Write a byte to I2C bus. Return 0 if ack by the slave.
 bool write_byte(unsigned char byte)
 {
   unsigned bit;
   bool     nack;

   for (bit = 0; bit < 8; ++bit)
   {
     write_bit((byte & 0x80) != 0);
     byte <<= 1;
   }

   nack = read_bit();

   SCCB_SID_H();

   return nack;
 }

 // Read a byte from I2C bus
 unsigned char read_byte(bool nack)
 {
   unsigned char byte = 0;
   unsigned char bit;

   data_in();
   for (bit = 0; bit < 8; ++bit) {
     byte |= read_bit();
     byte <<= 1;
   }

   write_bit(nack);
   SCCB_SID_H();

   data_out();

   return byte;
}

void write_reg(uint8_t reg, uint8_t *pData)
{
	init();

	start();

	/*send slave addres*/
	write_byte(WRITE_ADDR);

	/*send register addres*/
	write_byte(reg);

	/*send data address*/
	write_byte(*pData);

	stop();
}

 void read_reg(uint8_t reg, uint8_t *pData)
 {
 	init();

 	start();

 	write_byte(WRITE_ADDR);
 	write_byte(reg);

 	stop();
 	start();

 	write_byte(READ_ADDR);

 	*pData = read_byte(0);

 	stop();
 }

 uint8_t sccb_write(int regID, int regDat)
 {
//	 if (!sccb_init) init_sccb();
	 write_reg(regID, regDat);
	 return 0;
 }

 uint8_t sccb_read(uint8_t regID, uint8_t* regDat)
 {
//	 if (!sccb_init) init_sccb();
	 read_reg(regID, regDat);
	 return 0;
 }

// static void start(void)
//{
//    SCCB_SID_H();
//    delay_us(I2C_TIM);
//    SCCB_SIC_H();
//    delay_us(I2C_TIM);
//    SCCB_SID_L();
//    delay_us(I2C_TIM);
//    SCCB_SIC_L();
//    delay_us(I2C_TIM);
//}


//static void stop(void)
//{
//    SCCB_SID_L();
//    delay_us(I2C_TIM);
//    SCCB_SIC_H();
//    delay_us(I2C_TIM);
//    SCCB_SID_H();
//    delay_us(I2C_TIM);
//}


//static void sccb_bus_send_noack(void)
//{
//	SCCB_SID_H();
//	delay_us(I2C_TIM);
//	SCCB_SIC_H();
//	delay_us(I2C_TIM);
//	SCCB_SIC_L();
//	delay_us(I2C_TIM);
//	SCCB_SID_L();
//	delay_us(I2C_TIM);
//}
//
//static bool get_ack(void)
//{
//	SCCB_DATA_IN;
//	delay_us(I2C_TIM);
//
//	SCCB_SIC_H();
//	delay_us(I2C_TIM);
//
//	int r = HAL_GPIO_ReadPin(SCCB_SID_PORT, SCCB_SID_PIN);
//
//	SCCB_SID_L();
//	delay_us(I2C_TIM);
//
//	SCCB_SIC_L();
//	delay_us(I2C_TIM);
//
//	return r == 0;
//}
//
//static void send_ack(void)
//{
//	SCCB_SID_L();
//	delay_us(I2C_TIM);
//	SCCB_SIC_L();
//	delay_us(I2C_TIM);
//	SCCB_SIC_H();
//	delay_us(I2C_TIM);
//	SCCB_SIC_L();
//	delay_us(I2C_TIM);
//	SCCB_SID_L();
//	delay_us(I2C_TIM);
//}

//static void write_byte(uint8_t data)
//{
//	uint32_t i;
////	uint8_t tem;
//
//	for(i = 0; i < 8; i++)
//	{
//		if((data<<i) & 0x80) SCCB_SID_H();
//		else 				 SCCB_SID_L();
//
//		delay_us(I2C_TIM);
//		SCCB_SIC_H();
//		delay_us(I2C_TIM*2);
//		SCCB_SIC_L();
//		delay_us(I2C_TIM);
//	}
////	SCCB_DATA_IN;
////	delay_us(I2C_TIM);
////	SCCB_SIC_H();
////	delay_us(I2C_TIM);
////
////	tem = !SCCB_SID_STATE;
////
////	SCCB_SIC_L();
////	delay_us(I2C_TIM);
////	SCCB_DATA_OUT;
////	return tem;
//}

//static uint8_t read_byte(void)
//{
//	uint32_t i;
//	uint8_t read = 0;
//
//	SCCB_DATA_IN;
//	delay_us(I2C_TIM);
//	for(i = 8; i > 0; i--)
//	{
//		delay_us(I2C_TIM);
//		SCCB_SIC_H();
//
//		delay_us(I2C_TIM);
//		read = read << 1;
//
//		if(SCCB_SID_STATE) read += 1;
//
//		SCCB_SIC_L();
//		delay_us(I2C_TIM);
//	}
//    SCCB_DATA_OUT;
//	return read;
//}

//uint8_t sccb_write(int regID, int regDat)
//{
//	delay_us(5);
//	start();
//	write_byte(WRITE_ADDR);
//
//	if(!get_ack())
//	{
//		stop();
//		return 1;
//	}
//
//	delay_us(5);
//	write_byte(regID);
//
//	if(!get_ack())
//	{
//		stop();
//		return 2;
//	}
//
//	delay_us(5);
//	write_byte(regDat);
//
//	if(!get_ack())
//	{
//		stop();
//		return 3;
//	}
//
//	stop();
//	return 0;
//}
//
//uint8_t sccb_read(uint8_t regID, uint8_t* regDat)
//{
//	delay_us(10);
//
//	start();
//	write_byte(WRITE_ADDR);
//	if(!get_ack())
//	{
//		stop();
//		//goto start;
//		return 1;
//	}
//	delay_us(10);
//	write_byte(regID);
//	if(!get_ack())
//	{
//		stop();
//		//goto start;
//		return 2;
//	}
//	stop();
//	delay_us(10);
//	start();
//	write_byte(READ_ADDR);
//	if(!get_ack())
//	{
//		stop();
//		//goto start;
//		return 3;
//	}
//	delay_us(10);
//	*regDat = read_byte();
//	sccb_bus_send_noack();
//	stop();
//	return 0;
//}
