#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "gpios.h"
#include "key.h"

//ALIENTEK Mini STM32�����巶������8
//PWM���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
int io[] = {0,1,2,5,10,20,50,100,150,200};

#define ENCODER_PPR 1000  // ÿת������

volatile uint32_t encoder_count = 0;  // ������

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

    encoder_count++;
  }
}


 int main(void)
 {	
	u16 led0pwmval=0;  
int di;	 
	 int keyc=0;
	 int cmpr=0;
	u8 dir=1;	
	 KEY_Init();
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM1_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 
	 LN_Init();
	 di++;
		 IN1H();
		 IN2L();
//	 while(1){
//		keyc = KEY_Scan(0);
//		 if(keyc == KEY1_PRES){
//			 cmpr+=20;
//		 }else if(keyc == KEY0_PRES){
//			 cmpr -=20;
//		 }
//		 TIM_SetCompare1(TIM1,600); //4.8V -- 260, 0r/s

//	 }
	 
//	 while(1){
//		 keyc = KEY_Scan(1);
//		 if(keyc==KEY0_PRES){
//			 IN1H();
//			 IN2L();
//		 }else if(keyc==KEY1_PRES){
//			 IN1L();
//			 IN2H();
//		 }else{
//			 IN1H();
//			 IN2H();
//		 }
//	 }
//	 		IN1H();
//		 IN2L();
//		 delay_ms(5000);
//	 while(1){
//			IN2H();
//		 IN1L();
//		 delay_ms(10000);
//		IN1H();
//		 IN2L();
//		 delay_ms(10000);
//	 }
//	 while(1){
//		 delay_ms(3000);
//		 switch (di) {
//			 case 1:
//				 IN1H();
//					IN2L();
//			 di = 2;
//			 break;
//			 case 2:
//				 IN1L();
//			 IN2H();
//			 di=1;
//			 default:
//				 break;
//		 }
//	 }
	 
//	 while (1) {
//		 if(KEY_Scan(0) == KEY1_PRES){
//			 led0pwmval--;
//			 if(led0pwmval==0) led0pwmval=9;
//		 }
//			 TIM_SetCompare1(TIM1,io[led0pwmval]);
//				 IN1H();
//					IN2L();		 
//	 }
	 
   	while(1)
	{
 		delay_ms(3000);	 
 		if(led0pwmval>10)dir=0;
		if(led0pwmval==0)dir=1;	 
		if(dir)led0pwmval++;
		else led0pwmval--;	 		
		TIM_SetCompare1(TIM1,0);	   
	} 

}

