#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/5/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved											  
////////////////////////////////////////////////////////////////////////////////// 

void KEY_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOC.1,KEY1-->GPIOC.13  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//初始化 WK_UP-->GPIOA.0	  下拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
 
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	if(key_up&&(KEY0==0||KEY1==0||KEY2==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)
		{
			 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
			return 1;
		}
		else if(KEY1==0)
		{ 
	     	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
			return 2;
		}
		else if(KEY2==1)
		{
	    	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
			return 3;
		}
	}else if(KEY0==1&&KEY1==1&&KEY2==0)key_up=1; 	    
 
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	return 0;// 无按键按下
}
