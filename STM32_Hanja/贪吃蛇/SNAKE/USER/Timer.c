#include "Timer.h"


void TIME_Configuration(void)//配置TIM6
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //设置了在下一个更新事件装入活动的自动重装载寄存器周期的值 范围是0x0000-0xFFFF 
	TIM_TimeBaseStructure.TIM_Prescaler =(36000-1); //设置了用来作为 TIMx 时钟频率除数的预分频值 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_Cmd(TIM4, ENABLE);
} 


void Tick_Updata(uint32_t* tick)
{
    *tick=TIM_GetCounter(TIM4);
}

int If_TimeOut(uint32_t* oldtick,uint32_t diff_tick)  //return 1表示超时  return 0表示没有超时
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