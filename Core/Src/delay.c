#include "delay.h"

#include "core_cm7.h"

//static uint8_t  fac_us=0;
//static uint16_t fac_ms=0;
//
//void delay_init()
//{
//	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
//	fac_us = SystemCoreClock/8000000;
//	fac_ms = (uint16_t)fac_us*1000;
//}
//
//void delay_us(uint32_t nus)
//{
//	uint32_t temp;
//	SysTick->LOAD = nus*fac_us;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//	SysTick->VAL =0X00;
//}

TIM_HandleTypeDef *htim;

void delay_us_init(TIM_HandleTypeDef *tim)
{
	htim = tim;
}

//#pragma GCC push_options
//#pragma GCC optimize ("O3")
void delay_us(uint16_t us)
{
	uint16_t ticks = us * 96;
	__HAL_TIM_SET_COUNTER(htim, 0);
	while(__HAL_TIM_GET_COUNTER(htim) < ticks);
//	volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
//	volatile uint32_t start = DWT->CYCCNT;
//	while(DWT->CYCCNT - start < cycles);

}
//#pragma GCC pop_options

void delay_hns(uint16_t hns)
{
	uint16_t ticks = (hns * 97)/10;
	__HAL_TIM_SET_COUNTER(htim, 0);
	while(__HAL_TIM_GET_COUNTER(htim) < ticks);
//	volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
}
