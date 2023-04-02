#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

typedef struct
{
	 //相关速度PID参数
	float Velcity_Kp;
	float Velcity_Ki;
	float Velcity_Kd;
	float Ur;				//限幅值
	
	u8 PID_is_Enable;		//PID使能
	int Un;					//期望输出值
	int En_1;				//上一次的误差值
	int En_2;				//上上次的误差值
	int PWM;				//输出PWM值
	
}PID_InitDefStruct;

void PID_Init(PID_InitDefStruct* p);
void Velocity_PID(int TargetVelocity,int CurrentVelocity,PID_InitDefStruct* p);

#endif
