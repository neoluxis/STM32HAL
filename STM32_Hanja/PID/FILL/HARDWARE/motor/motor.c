#include "sys.h" 
#include "motor.h"
#include "main.h"

/**************************************************************************
���ܣ���PID������PWMֵ��ֵ��PWM�Ĵ���
����������PWMֵ
**************************************************************************/
//void Motor_SetPwm(int PWM_Left)
//{
//	if(PWM_Left>=0)
//	{
//		TIM_SetCompare1(TIM2,PWM_Left);//����ռ�ձ�
//		TIM_SetCompare2(TIM2,0);
//	}
//	else
//	{
//		TIM_SetCompare1(TIM2,0);
//		TIM_SetCompare2(TIM2,myabs(PWM_Left));
//	}
//}
void Motor_SetPwm(int PWM_Left)
{
	if(PWM_Left>=0)
	{
		Motor_SetDir(TUDUKE);
		TIM_SetCompare1(TIM1,myabs(PWM_Left)); 

	}
	else
	{
		Motor_SetDir(RETREAT);
		TIM_SetCompare1(TIM1,myabs(PWM_Left)); 

	}
}

// dir: 0 posi, 1 nega, 2 speed down, 3 no power
void Motor_SetDir(int dir){
	switch(dir){
		case 0:
			IN1 = 1;
			IN2 = 0;
			break;
		case 1:
			IN1 = 0;
			IN2 = 1;
			break;
		case 2:
			IN1 = 1;
			IN2 = 1;
			break;
		case 3:
			IN1 = 0;
			IN2 = 0;
			break;
		default:
			break;
	}
}

void Motor_DirInit(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(IN1_RCC, ENABLE);	 //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = IN1_Pin;				 //IN1-->PA.1 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(IN1_Port, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.1
  GPIO_SetBits(IN1_Port, IN1_Pin);						 //PA.1 �����
	
	RCC_APB2PeriphClockCmd(IN2_RCC, ENABLE);	 //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = IN2_Pin;				 //IN2-->PA.2 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(IN2_Port, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.2
  GPIO_SetBits(IN2_Port, IN2_Pin);						 //PA.2 �����
	
	Motor_SetDir(3);
}
	
/**************************************************************************
���ܣ�ȡ����ֵ
��������Ҫȡ����ֵ����
**************************************************************************/
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}

