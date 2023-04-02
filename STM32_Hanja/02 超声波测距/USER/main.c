#include "sys.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
#include "bmp.h"
#include "display.h"

extern u8  TIM3CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u16 TIM3CH3_CAPTURE_VAL;	//���벶��ֵ


int main(void)
{
  u32 temp=10000;
  u16 cnt=0,ccr=0;    
  u8 dis0zs=0;//��������
  u8 dis0xs=5;
  u8 dis1zs=0;//ʵ�ʾ���
  u8 dis1xs=25;
  u8 keyval=0;
  float dis;
  u16 dis0;
  
	delay_init();	    		  
	LED_Init();	
  KEY_Init();
  OLED_Init();
  
  NVIC_Configuration();//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  uart_init(9600);
	TIM1_PWM_Init(899,39);//����Ƶ PWMƵ��=2K 
  TIM3_Cap_Init(0XFFFF,72-1);//��1Mhz��Ƶ�ʼ��� 
  
  while(1)
	{		
		TRIG=1;//�����ź�
		delay_ms(100);
		TRIG=0;
		
 		delay_ms(10);

    TIM_SetCompare1(TIM1,cnt);
		
		if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM3CH3_CAPTURE_STA&0X3F;//�������
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM3CH3_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
	
 			TIM3CH3_CAPTURE_STA=0;//������һ�β���
      printf("temp:%d\n",temp);
      dis=temp*0.00017;
      dis1zs=dis/1;
      dis1xs=(dis-dis1zs)*100;
      printf("ʵ�ʾ��룺%.2fm\r\n",dis);
      
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
