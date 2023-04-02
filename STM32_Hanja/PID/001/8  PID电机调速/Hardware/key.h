#ifndef __KEY_H
#define __KEY_H

//#define USE_WK_UP // whether to use WK_UP key

#define K0_RCC			RCC_APB2Periph_GPIOC
#define K1_RCC			RCC_APB2Periph_GPIOC
#define WK_UP_RCC		RCC_APB2Periph_GPIOA
#define K0_Port			GPIOC
#define K1_Port			GPIOC
#define WK_UP_Port	GPIOA
#define K0_Pin			GPIO_Pin_1
#define K1_Pin			GPIO_Pin_13
#define WK_UP_Pin		GPIO_Pin_0

#define KEY0  GPIO_ReadInputDataBit(K0_Port,K0_Pin)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(K1_Port,K1_Pin)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(WK_UP_Port,WK_UP_Pin)//读取按键2 

#define K0_Pres			1
#define K1_Pres			2
#define WK_UP_Pres	3
#define NON_Pres		0

void KEY_Init(void);
int KEY_Scan(int);

#endif
