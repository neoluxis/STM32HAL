#ifndef __LED_H
#define __LED_H

#include "main.h"

#define L0_RCC	RCC_APB2Periph_GPIOA
#define L1_RCC	RCC_APB2Periph_GPIOC
#define L0_PORT	GPIOA
#define L1_PORT	GPIOC
#define L0_Pin	GPIO_Pin_8
#define L1_Pin	GPIO_Pin_11

#define LED0_ON()		GPIO_ResetBits(L0_PORT, L0_Pin)
#define LED1_ON()		GPIO_ResetBits(L1_PORT, L1_Pin)
#define LED0_OFF()	GPIO_SetBits(L0_PORT, L0_Pin)
#define LED1_OFF()	GPIO_SetBits(L1_PORT, L1_Pin)
#define LED0_TOG()	
#define LED1_TOG()	

void LED_Init(void);

#endif
