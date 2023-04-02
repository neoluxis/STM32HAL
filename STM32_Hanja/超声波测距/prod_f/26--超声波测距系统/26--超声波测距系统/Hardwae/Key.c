#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "key.h"

void Key_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOC.1,KEY1-->GPIOC.13  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//初始化 WK_UP-->GPIOA.0	  下拉输入
 //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	int KeyNum = -1;
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 0)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 0);
		Delay_ms(10);
		
		KeyNum = KEY0_PRES; 
	}
	
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0);
		Delay_ms(10);
		
		KeyNum = KEY1_PRES;
	}
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
	{
		Delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) == 1);
		Delay_ms(10);
		
		KeyNum = WK_UP_PRES;
	}	
	return KeyNum;
}
