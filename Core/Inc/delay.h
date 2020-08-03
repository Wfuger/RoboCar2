#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>

#include "main.h"

void delay_us_init(TIM_HandleTypeDef *tim);
void delay_us(uint16_t us);
void delay_hns(uint16_t hns);

#endif // __DELAY_H__
