#include "sys.h"
#include "pid.h"

/**************************************************************************
功能：PID结构体初始化
函数：void PID_Init(PID_InitDefStruct* p)
参数：p:PID结构体（地址）
**************************************************************************/
void PID_Init(PID_InitDefStruct* p)
{
	p->Velcity_Kp = 5.2;
	p->Velcity_Ki = 0.72;
	p->Velcity_Kd = 0;
	p->Ur = 7100;
	p->PID_is_Enable = 1;
	p->Un = 0;
	p->En_1 = 0;
	p->En_2 = 0;
	p->PWM = 0;
}

/**************************************************************************
功能：PID控制算法，将输出PWM赋值给PID结构体的PWM
函数：void Velocity_PID(int TargetVelocity,int CurrentVelocity,PID_InitDefStruct* p)
参数：TargetVelocity:期望速度速度  CurrentVelocity:实际速度  p:PID结构体（地址）
**************************************************************************/

void Velocity_PID(int TargetVelocity,int CurrentVelocity,PID_InitDefStruct* p)
{
	if(p->PID_is_Enable == 1)
	{
		int En = TargetVelocity - CurrentVelocity;//误差值                                                     
	
		p->Un += p->Velcity_Kp*(En - p->En_1) + p->Velcity_Ki*En + p->Velcity_Kd*(En - 2*p->En_1 + p->En_2);//增量式PID
		
		p->En_2=p->En_1;
		p->En_1=En;
		
		p->PWM = p->Un;
		
		/*输出限幅*/
		if(p->PWM>p->Ur) p->PWM=p->Ur;
		if(p->PWM<-p->Ur) p->PWM=-p->Ur;
	}
	else
	{
		PID_Init(p);
	}
	
}


