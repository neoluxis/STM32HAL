#include "delay.h"
#include "led.h"
#include "sys.h"
#include "time.h"
#include "oled.h"
extern u8  TIM5CH1_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
 int main(void)
 {	
	u32 temp=0;
	LED_Init();
	delay_init();
	OLED_Init();			//初始化OLED      
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
   while(1)
	 {	
		 tri=1;
		 delay_us(20);
		 tri=0;
		 delay_us(20);		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			OLED_ShowNum(15,15,temp*170/10000,6,16);
			OLED_Refresh_Gram();	
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
	}
	 }

