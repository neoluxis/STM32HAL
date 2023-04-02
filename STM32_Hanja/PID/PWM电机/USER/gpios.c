#include "stm32f10x.h"                  // Device header
#include "gpios.h"

void LN_Init(){
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	g.GPIO_Mode = GPIO_Mode_Out_PP;
	g.GPIO_Pin = GPIO_Pin_3;
	g.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &g);
	g.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &g);
	IN1H();
	IN2H();
}


