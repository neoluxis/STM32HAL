#include "LED.h"
#include "stm32f10x.h"

void LED_Init(){
	GPIO_InitTypeDef str;
	str.GPIO_Mode = GPIO_Mode_Out_PP;
	str.GPIO_Pin = GPIO_Pin_8;
	str.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &str);
	str.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &str);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void LED0_Toggle(){
	int static light = 0;
	switch(light){
		case 0:
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);
			light = 1;
			break;
		case 1:
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
			light = 0;
			break;
		default:
			break;
	}
}
