#include "timer.h"                // Device header
#include "stm32f10x.h"                  // Device header

void TIM6_Init(u16 arr,u16 psc){
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//ʹ�ܶ�ʱ��ʱ��
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;//Ԥװ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;//Ԥ��Ƶϵ��
	 
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);//��ʼ����ʱ��������ARR,PSC
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//������ʱ���ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVIC_InitStruct);//����NVIC
	
	TIM_Cmd(TIM6,ENABLE);//ʹ�ܶ�ʱ��
}	



