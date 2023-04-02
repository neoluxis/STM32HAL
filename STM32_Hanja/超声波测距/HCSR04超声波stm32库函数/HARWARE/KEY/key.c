#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

/*************************PC13（K2键）,PE0（k1键）上拉输入（无信号时为高电平）***********************/


void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//PE0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);//PC13
}

