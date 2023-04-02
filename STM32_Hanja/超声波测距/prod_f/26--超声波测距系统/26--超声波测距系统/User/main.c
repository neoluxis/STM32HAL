#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "CSB_SR04.h"
#include "Key.h"
#include "PWM.h"
#include "Serial.h"
#include "stdio.h"

uint16_t measured; 		   //距离 
uint16_t Knum;  //按键反馈接收
uint16_t ALERT = 8;	   //警报距离 (cm)

void OLED_UpdateDistance(void) //ok
{
	if(Knum == KEY0_PRES){
		ALERT++;
	}
	if(Knum == KEY1_PRES){
		ALERT--;
	}
	OLED_ShowDouble(73, 1, measured/100.0);
	OLED_ShowDouble(73, 17, ALERT/100.0);
}

void LED_ALERT(void)
{ 
	int percentage = 100 * (((ALERT>8)?2*ALERT:15) - measured) / (((ALERT>7)?2*ALERT:15) - ALERT);
	if(percentage >= 100) percentage = 100;
	if(percentage < 0) percentage = 0;
	PWM_SetCompare4(100 - percentage);				//Duty = CCR / 100
	OLED_ShowNum(81, 33, percentage, 3,16);
}

void ToUSART(void) //ok
{
	static int t=0;
	if(t==30) t = 0;
	t++;
	Serial_Printf("\r\n%2d:distance=%3dcm", t,measured);
	Serial_Printf("\r\n");
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SPI2_Init();
	OLED_Init();
	OLED_ShowFrame();
	CSB_SR04_Init();
	Key_Init();
	PWM_Init();
	Serial_Init();
	PWM_SetPrescaler(360 - 1);  //2kHz	//Freq = 72M / (PSC + 1) / 100	
	
	while (1)
	{
		Knum = Key_GetNum();
		measured = Distance();
		OLED_UpdateDistance();
		LED_ALERT();
		ToUSART();
	}
}
