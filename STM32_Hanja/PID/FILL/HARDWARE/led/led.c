#include "led.h"
#include "main.h"

void LED_Init(void){ 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(L0_RCC, ENABLE);	 //使能PA,PD端口时钟
  GPIO_InitStructure.GPIO_Pin = L0_Pin;				 //LED0-->PA.8 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(L0_PORT, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
  GPIO_SetBits(L0_PORT, L0_Pin);						 //PA.8 输出高
	
  RCC_APB2PeriphClockCmd(L1_RCC, ENABLE);	 //使能PA,PD端口时钟
  GPIO_InitStructure.GPIO_Pin = L1_Pin;	    		 //LED1-->PD.2 端口配置, 推挽输出
  GPIO_Init(L1_PORT, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
  GPIO_SetBits(L1_PORT, L1_Pin); 						 //PD.2 输出高 	
}
