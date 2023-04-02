#include "CSB_SR04.h"
#include "Delay.h"

uint16_t N = 0;
float distance = 0;

void CSB_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitSructure; //GPIO��ʼ��
	NVIC_InitTypeDef NVIC_InitSructure; //��ʱ����ʼ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStrure;  //�ⲿ�жϳ�ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��PB�˿�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//PB6�����PB7����
	GPIO_InitSructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitSructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitSructure);
	
	GPIO_InitSructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitSructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitSructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitSructure);
	
	//���ʱ��tim=(arr+1)*(psc+1)/72000000
	TIM_TimeBaseStrure.TIM_Period = 49999;
	TIM_TimeBaseStrure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStrure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStrure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStrure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  //ʹ���ж�
	NVIC_InitSructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitSructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
	NVIC_InitSructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitSructure.NVIC_IRQChannelCmd = ENABLE;  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitSructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		N += 1;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}

int count = 0;
float Distance(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);  //Ԥ�������������
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);  //����10us����
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0);
	TIM2->CNT=0;
	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 1);
	count = TIM2->CNT;
	
	distance = (float)count/58;
	return distance;
}


