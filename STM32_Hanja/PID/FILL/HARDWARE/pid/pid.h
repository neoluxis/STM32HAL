#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

typedef struct
{
	 //����ٶ�PID����
	float Velcity_Kp;
	float Velcity_Ki;
	float Velcity_Kd;
	float Ur;				//�޷�ֵ
	
	u8 PID_is_Enable;		//PIDʹ��
	int Un;					//�������ֵ
	int En_1;				//��һ�ε����ֵ
	int En_2;				//���ϴε����ֵ
	int PWM;				//���PWMֵ
	
}PID_InitDefStruct;

void PID_Init(PID_InitDefStruct* p);
void Velocity_PID(int TargetVelocity,int CurrentVelocity,PID_InitDefStruct* p);

#endif
