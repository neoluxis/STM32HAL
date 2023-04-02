#include "sys.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
#include "bmp.h"
#include "display.h"

extern u8  TIM3CH3_CAPTURE_STA;		//输入捕获状态		    				
extern u16 TIM3CH3_CAPTURE_VAL;	//输入捕获值


int main(void)
{
  u32 temp=10000;
  u16 cnt=0,ccr=0;    
  u8 dis0zs=0;//报警距离
  u8 dis0xs=5;
  u8 dis1zs=0;//实际距离
  u8 dis1xs=25;
  u8 keyval=0;
  float dis;
  u16 dis0;
  
	delay_init();	    		  
	LED_Init();	
  KEY_Init();
  OLED_Init();
  
  NVIC_Configuration();//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart_init(9600);
	TIM1_PWM_Init(899,39);//不分频 PWM频率=2K 
  TIM3_Cap_Init(0XFFFF,72-1);//以1Mhz的频率计数 
  
  while(1)
	{		
		TRIG=1;//触发信号
		delay_ms(100);
		TRIG=0;
		
 		delay_ms(10);

    TIM_SetCompare1(TIM1,cnt);
		
		if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM3CH3_CAPTURE_STA&0X3F;//溢出次数
			temp*=65536;//溢出时间总和
			temp+=TIM3CH3_CAPTURE_VAL;//得到总的高电平时间
	
 			TIM3CH3_CAPTURE_STA=0;//开启下一次捕获
      printf("temp:%d\n",temp);
      dis=temp*0.00017;
      dis1zs=dis/1;
      dis1xs=(dis-dis1zs)*100;
      printf("实际距离：%.2fm\r\n",dis);
      
      if(dis<(dis0zs+dis0xs*0.01)) 
        ccr=0;
		  if(dis<(dis0zs+dis0xs*0.01+0.15)&&dis>(dis0zs+dis0xs*0.01)) 
        ccr=(dis-(dis0zs+dis0xs*0.01))/0.15*100;
		  if(dis>(dis0zs+dis0xs*0.01+0.15)) 
        ccr=100;
      
      cnt=(100-ccr)*9;
      printf("cnt:%d\r\n",cnt);
    }
    
    display(dis1zs,dis1xs,dis0zs,dis0xs,cnt);
    
    dis0=dis0zs*100+dis0xs;
    if(KEY_Scan(1)==1)
    {
      while(KEY_Scan(1)==2)
      {
        keyval++;
      }
    }
    printf("dis0=%d\n",dis0);
    switch(keyval)
    {
      case 1:dis0=key_jia(dis0);break;
      case 2:dis0=key_jian(dis0);break;
    }
    dis0zs=dis0/100;
    dis0xs=dis0%100;
  }
}
