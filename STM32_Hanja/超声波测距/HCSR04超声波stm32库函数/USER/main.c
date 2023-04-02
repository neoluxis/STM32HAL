#include "delay.h"
#include "led.h"
#include "sys.h"
#include "time.h"
#include "oled.h"
extern u8  TIM5CH1_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
 int main(void)
 {	
	u32 temp=0;
	LED_Init();
	delay_init();
	OLED_Init();			//��ʼ��OLED      
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
   while(1)
	 {	
		 tri=1;
		 delay_us(20);
		 tri=0;
		 delay_us(20);		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			OLED_ShowNum(15,15,temp*170/10000,6,16);
			OLED_Refresh_Gram();	
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
		}
	}
	 }

