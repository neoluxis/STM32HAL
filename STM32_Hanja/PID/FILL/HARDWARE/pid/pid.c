#include "sys.h"
#include "pid.h"

/**************************************************************************
���ܣ�PID�ṹ���ʼ��
������void PID_Init(PID_InitDefStruct* p)
������p:PID�ṹ�壨��ַ��
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
���ܣ�PID�����㷨�������PWM��ֵ��PID�ṹ���PWM
������void Velocity_PID(int TargetVelocity,int CurrentVelocity,PID_InitDefStruct* p)
������TargetVelocity:�����ٶ��ٶ�  CurrentVelocity:ʵ���ٶ�  p:PID�ṹ�壨��ַ��
**************************************************************************/

void Velocity_PID(int TargetVelocity,int CurrentVelocity,PID_InitDefStruct* p)
{
	if(p->PID_is_Enable == 1)
	{
		int En = TargetVelocity - CurrentVelocity;//���ֵ                                                     
	
		p->Un += p->Velcity_Kp*(En - p->En_1) + p->Velcity_Ki*En + p->Velcity_Kd*(En - 2*p->En_1 + p->En_2);//����ʽPID
		
		p->En_2=p->En_1;
		p->En_1=En;
		
		p->PWM = p->Un;
		
		/*����޷�*/
		if(p->PWM>p->Ur) p->PWM=p->Ur;
		if(p->PWM<-p->Ur) p->PWM=-p->Ur;
	}
	else
	{
		PID_Init(p);
	}
	
}


