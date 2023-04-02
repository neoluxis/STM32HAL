#include "led.h"
#include "main.h"

void LED_Init(void){ 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(L0_RCC, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = L0_Pin;				 //LED0-->PA.8 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(L0_PORT, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
  GPIO_SetBits(L0_PORT, L0_Pin);						 //PA.8 �����
	
  RCC_APB2PeriphClockCmd(L1_RCC, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = L1_Pin;	    		 //LED1-->PD.2 �˿�����, �������
  GPIO_Init(L1_PORT, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
  GPIO_SetBits(L1_PORT, L1_Pin); 						 //PD.2 ����� 	
}
