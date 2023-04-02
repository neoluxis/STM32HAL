#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "CSB_SR04.h"
#include "Key.h"
#include "PWM.h"
#include "Serial.h"

uint16_t con = 0; 		   //距离
uint16_t Knum, Knum1 = 0;  //按键反馈接收
uint16_t ALERT = 15;	   //警报距离
int16_t Change = -1;	   //待修改警报距离


void OLED_UI(void)
{
	OLED_ShowChinese(1,1,3);  //距
	OLED_ShowChinese(1,3,4);  //离
	OLED_ShowChinese(1,13,5);  //米
	OLED_ShowString(1,6,":  .");
	
	OLED_ShowChinese(3,1,6);  //报
	OLED_ShowChinese(3,3,7);  //警
	OLED_ShowChinese(3,5,3);  //距
	OLED_ShowChinese(3,7,4);  //离
	OLED_ShowString(3,10,":  .");
	
	OLED_ShowString(2,1,"Duty =    %");
}

void OLED_Data(void)
{
	OLED_ShowNum(1, 8, con/100, 1);
	OLED_ShowNum(1, 10, con%100, 2);
	
	OLED_ShowNum(3, 12, ALERT/100, 1);
	OLED_ShowNum(3, 14, ALERT%100, 2);
	
	if((Knum1 == 1) | (Knum1 == 3))
	{
		OLED_ShowChinese(4,1,8);  //修
		OLED_ShowChinese(4,3,9);  //改
		OLED_ShowChinese(4,13,5);  //米
		OLED_ShowString(4,6,":  .");
		OLED_ShowNum(4, 8, Change/100, 1);
		OLED_ShowNum(4, 10, Change%100, 2);		
	}
}

void con_Change(void)
{
	if(Knum == 2)
	{
		Change++;
	}
	
	if(Knum == 3)
	{
		Change = Change + 10;
	}
	
	if(Knum == 4)  //确认
	{
		ALERT = Change;
		Change = -1;
		Knum1 = 0;
		OLED_ShowString(4,1,"                ");
	}
	
	if(Knum == 5)  //取消
	{
		Change = -1;
		Knum1 = 0;
		OLED_ShowString(4,1,"                ");
	}
}

void LED_ALERT(void)
{
	if(con <= ALERT)
	{
		PWM_SetCompare4(0);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 100, 3);
	}
	
	if((con <= ALERT + 3) & (con > ALERT + 0))
	{
		PWM_SetCompare4(10);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 90, 3);
	}
	
	if((con <= ALERT + 6) & (con > ALERT + 3))
	{
		PWM_SetCompare4(20);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 80, 3);
	}
	
	
	if((con <= ALERT + 9) & (con > ALERT + 6))
	{
		PWM_SetCompare4(40);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 60, 3);
	}
	
	if((con > ALERT + 9) & (con <= ALERT + 12))
	{
		PWM_SetCompare4(60);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 40, 3);
	}
	
	if((con <= ALERT + 15) & (con > ALERT + 12))
	{
		PWM_SetCompare4(90);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 10, 3);
	}
	
	if(con > (ALERT + 15))
	{
		PWM_SetCompare4(100);				//Duty = CCR / 100
		OLED_ShowNum(2, 8, 0, 3);
	}
}

void Serial_TR(void)
{
	Serial_Printf("\r\ndistance=%d", con);  //文本模式
	Serial_Printf("\r\nALERT=%d", ALERT);  //文本模式
	Serial_Printf("\r\n");
	
//	Serial_Printf("\r\n%d", con);  //文本模式
	
	if (Serial_GetRxFlag() == 1)
	{
		Change = Serial_GetRxData();
		Change = Change - (Change / 16) * 6;  //串口发回十六进制数转为同数字的十进制数
		Knum1 = 3;
	}
	
	if(Knum1 == 3)
	{
		if(Knum == 4)  //确认
		{
			ALERT = Change;
			Change = -1;
			Knum1 = 0;
			OLED_ShowString(4,1,"                ");
		}
		
		if(Knum == 5)  //取消
		{
			Change = -1;
			Knum1 = 0;
			OLED_ShowString(4,1,"                ");
		}	
	}	
}

void Knum_ON(void)
{
	if((Knum == 2) && (Knum1 == 0))
	{
		Knum1 = 1;
	}

	if(Knum1 == 1)  //进入修改报警距离程序
	{
		con_Change();
	}
	
	Delay_ms(200);
	
}

void HEART(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	OLED_UI();
	CSB_SR04_Init();
	Key_Init();
	PWM_Init();
	Serial_Init();
	
	PWM_SetPrescaler(360 - 1);  //2kHz	//Freq = 72M / (PSC + 1) / 100	
}


int main(void)
{
	HEART();	
	while (1)
	{
		Knum = Key_GetNum();
		con = Distance();
		OLED_Data();
		LED_ALERT();
		Serial_TR();
		Knum_ON();	
	}
}
