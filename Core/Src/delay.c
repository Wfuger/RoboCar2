#include "delay.h"

#include "core_cm7.h"

TIM_HandleTypeDef *htim;

void delay_us_init(TIM_HandleTypeDef *tim)
{
	htim = tim;
}

//#pragma GCC push_options
//#pragma GCC optimize ("O3")
void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(htim, 0);
	while(__HAL_TIM_GET_COUNTER(htim) < us);
//	volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
//	volatile uint32_t start = DWT->CYCCNT;
//	while(DWT->CYCCNT - start < cycles);

}
//#pragma GCC pop_options
