#include "key.h"
#include "stm32f10x.h"                  // Device header
#include "delay.h"

void KEY_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(K0_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = K0_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(K0_Port, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(K1_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = K1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(K1_Port, &GPIO_InitStructure);

#ifdef USE_WK_UP
 	RCC_APB2PeriphClockCmd(WK_UP_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = WK_UP_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(WK_UP_Port, &GPIO_InitStructure);
#endif
}

// mode: 0 not support, 1 support
int KEY_Scan(int mode)
{	 
	static u8 key_up=1;
	if(mode)key_up=1; 	// keep press enabled	  
#ifdef USE_WK_UP
	if(key_up && (KEY0==0||KEY1==0||WK_UP==1))
	{
		Delay_ms(10); 
		key_up=0;
		if(KEY0==0)return K0_Pres;
		else if(KEY1==0)return K1_Pres;
		else if(WK_UP==1)return WK_UP_Pres; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
#else
	if(key_up && (KEY0==0||KEY1==0))
	{
		Delay_ms(10); 
		key_up=0;
		if(KEY0==0)return K0_Pres;
		else if(KEY1==0)return K1_Pres;
	}else if(KEY0==1&&KEY1==1)key_up=1; 
#endif
	return NON_Pres;
}

