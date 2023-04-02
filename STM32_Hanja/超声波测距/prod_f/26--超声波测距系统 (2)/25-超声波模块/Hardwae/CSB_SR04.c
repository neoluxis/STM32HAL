#include "CSB_SR04.h"
#include "Delay.h"

uint16_t N = 0;
float distance = 0;

void CSB_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitSructure; //GPIO初始化
	NVIC_InitTypeDef NVIC_InitSructure; //定时器初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStrure;  //外部中断初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能PB端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//PB6输出，PB7输入
	GPIO_InitSructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitSructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitSructure);
	
	GPIO_InitSructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitSructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitSructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitSructure);
	
	//溢出时间tim=(arr+1)*(psc+1)/72000000
	TIM_TimeBaseStrure.TIM_Period = 49999;
	TIM_TimeBaseStrure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStrure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStrure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStrure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  //使能中断
	NVIC_InitSructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitSructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级2级
	NVIC_InitSructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitSructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ通道使能
	NVIC_Init(&NVIC_InitSructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		N += 1;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

int count = 0;
float Distance(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);  //预先拉低输出引脚
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);  //产生10us脉冲
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0);
	TIM2->CNT=0;
	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 1);
	count = TIM2->CNT;
	
	distance = (float)count/58;
	return distance;
}


