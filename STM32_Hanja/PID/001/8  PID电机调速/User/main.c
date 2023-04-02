#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Encoder.h"
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"
#include "Key.h"
#include "PID.h"
#include "Serial.h"


int Rate;
int Set=40;
uint8_t Time;
int Temp;
int    *p=&Temp;

int main(void)
{
    Timer_Init();
    PWM_Init();
    
    OLED_Init();
    Encoder_Init();
    Key_Init();
    Serial_Init();
    
    while (1)
    {
        if(Key_GetNum()){Set-=20;}
        
        OLED_ShowString(0,0,"Rate:");
        OLED_ShowNum(40,0,Rate,5,16);     //实际转速
        
        OLED_ShowString(0,2,"Set:");
        OLED_ShowNum(40,2,Set,5,16);     //设定期望转速
       
        Temp=(int)PID(Set,Rate);
        OLED_ShowNum(40,6,Temp,5,16);

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
