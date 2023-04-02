#include "Timer.h"


void TIME_Configuration(void)//����TIM6
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ ��Χ��0x0000-0xFFFF 
	TIM_TimeBaseStructure.TIM_Prescaler =(36000-1); //������������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_Cmd(TIM4, ENABLE);
} 


void Tick_Updata(uint32_t* tick)
{
    *tick=TIM_GetCounter(TIM4);
}

int If_TimeOut(uint32_t* oldtick,uint32_t diff_tick)  //return 1��ʾ��ʱ  return 0��ʾû�г�ʱ
{
   uint32_t ticknow;
   uint32_t diff;
	
    ticknow=TIM_GetCounter(TIM4);
    diff=ticknow-(*oldtick);
		return diff > diff_tick;
}
void Delay_ms(uint32_t ms)
{
	uint32_t temp=0;
	
	Tick_Updata(&temp);
	while(!If_TimeOut(&temp,ms));
}