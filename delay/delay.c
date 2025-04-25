#include "delay.h"



void delay_ms(uint16_t ms)
{
	TIM_SetCounter(TIM3, 0);
	while(TIM_GetCounter(TIM3) < ms) {}
}


void Tim_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period				 = 65536 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler		 = 36000 - 1;				//1ms
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM3, ENABLE);
}