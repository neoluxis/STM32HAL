#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

#define IN1 			PAout(3)
#define IN2 			PAout(2)
#define IN1_RCC		RCC_APB2Periph_GPIOA
#define IN2_RCC		RCC_APB2Periph_GPIOA
#define IN1_Port	GPIOA
#define IN1_Pin		GPIO_Pin_3
#define IN2_Port	GPIOA
#define IN2_Pin		GPIO_Pin_2

// Directions
#define TUDUKE 	0
#define RETREAT	1
#define BRAKE		2
#define NOPWR		3

void Motor_SetPwm(int PWM_Left);
void Motor_SetDir(int dir);
void Motor_DirInit(void);
int myabs(int a);


#endif

