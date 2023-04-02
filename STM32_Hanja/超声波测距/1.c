#include "timer.h"
#include "usart.h"
#include "sys.h"

//定时器4通道3输入捕获配置

TIM_ICInitTypeDef  TIM4_ICInitStructure;

void TIM4_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能TIM4时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  //PB8 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB8 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 //PB8 下拉
	
	//初始化定时器4 TIM4	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM2输入捕获参数
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; 
  	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4
 
}


u8  TIM4CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM4CH3_CAPTURE_VAL;	//输入捕获值
 
//定时器5中断服务程序	 
void TIM4_IRQHandler(void)
{ 

 	if((TIM4CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM4CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM4CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM4CH3_CAPTURE_VAL=0XFFFF;
				}else TIM4CH3_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//捕获1发生捕获事件
		{	
			if(TIM4CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM4CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				TIM4CH3_CAPTURE_VAL=TIM_GetCapture3(TIM4);
		   		TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM4CH3_CAPTURE_STA=0;			//清空
				TIM4CH3_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM4,0);
				TIM4CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
 
}

