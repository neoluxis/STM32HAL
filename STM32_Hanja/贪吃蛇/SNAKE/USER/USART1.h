#ifndef __USART1_INIT_H__
#define __USART1_INIT_H__

#include "stm32f10x.h"
#include <stdio.h>

int fputc(int ch, FILE *f);
void USART1_Configuration(void);//打印输出串口初始化

#endif
