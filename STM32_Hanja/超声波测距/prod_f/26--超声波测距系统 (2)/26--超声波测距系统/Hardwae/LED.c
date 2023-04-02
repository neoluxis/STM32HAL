#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_8); //�ϵ�Ĭ�ϸߵ�ƽ
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
}

void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3) == 0) //��ȡ�˿ڳ����ݺ���
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8) == 0) //��ȡ�˿ڳ����ݺ���
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	}
}
