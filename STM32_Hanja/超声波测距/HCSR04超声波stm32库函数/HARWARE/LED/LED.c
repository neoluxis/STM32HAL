#include "led.h"
void LED_Init(void)
	{
		
		GPIO_InitTypeDef  GPIO_InitStructure; 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能端口时钟
	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			    //端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	 GPIO_Init(GPIOD, &GPIO_InitStructure);		 //初始化GPIOD.4
		
	}

	