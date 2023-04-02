#ifndef __Key_H__
#define __Key_H__
#include "stm32f10x.h"                  // Device header

#define KEY0_PRES 0
#define KEY1_PRES 1
#define WK_UP_PRES 2

//#define KEY0 PCin(1)   //PA13
//#define KEY1 PCin(13)	//PA15 
//#define KEY2 PAin(0)	//PA0  WK_UP

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
