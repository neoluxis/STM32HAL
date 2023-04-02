#include "timer.h"                // Device header
#include "stm32f10x.h"                  // Device header

void TIM6_Init(u16 arr,u16 psc){
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//使能定时器时钟
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;//预装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;//预分频系数
	 
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);//初始化定时器，配置ARR,PSC
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//开启定时器中断
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVIC_InitStruct);//配置NVIC
	
	TIM_Cmd(TIM6,ENABLE);//使能定时器
}	



