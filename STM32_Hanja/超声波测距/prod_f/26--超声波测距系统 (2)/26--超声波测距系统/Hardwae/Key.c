#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //…œ¿≠ ‰»Î
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;  //K2; K3
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;  //K4; K5
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0);
		Delay_ms(10);
		
		KeyNum = 2;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0);
		Delay_ms(10);
		
		KeyNum = 3;
	}
	
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 0);
		Delay_ms(10);
		
		KeyNum = 4;
	}
	
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0);
		Delay_ms(10);
		
		KeyNum = 5;
	}
	
	return KeyNum;
}
