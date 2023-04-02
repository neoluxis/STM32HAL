#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Encoder.h"
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"
#include "key.h"
#include "PID.h"
#include "Serial.h"
#include "Delay.h"

int Rate;
int Set=40;
uint8_t Time;
int Temp;
int *p=&Temp;

int main(void)
{
    Timer_Init();
    PWM_Init();
    OLED_Init();
    Encoder_Init();
    KEY_Init();
	KEY_Init();
    Serial_Init();
    
    while (1)
    {
        switch(KEY_Scan(1)){
					case K0_Pres:
						Set += 2;  break;
					case K1_Pres:
						Set -= 2; break;
					default:
							break;
				}
        
        OLED_ShowString(0,0,"Crnt:");
        OLED_ShowNum(45,0,Rate,5,16);     //实际转速
        
        OLED_ShowString(0,2,"Tagt:");
        OLED_ShowNum(45,2,Set,5,16);     //设定期望转速
       
        Temp=(int)PID(Set,Rate);
//        OLED_ShowNum(45,6,Temp,5,16);

        Serial_Printf("\r\n%d",Rate);
        
        Delay_ms(100);

    
    }
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {  

        Rate=Encoder_Get();                                               //编码器测速
        
        Time++;
        if(Time==2){  PWM_SetCompare(50+(int)PID(Set,Rate));Time%=2;}          //PID反馈

       
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
