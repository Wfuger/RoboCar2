#include "delay.h"

#include "core_cm7.h"

#pragma GCC push_options
#pragma GCC optimize ("O3")
void delay_us(uint32_t us)
{
	/* Go to number of cycles for system */
	us *= (HAL_RCC_GetHCLKFreq() / 1000000);

	  /* Delay till end */
	for (uint32_t i = 0; i<us; ++i) asm("NOP");
}
#pragma GCC pop_options
