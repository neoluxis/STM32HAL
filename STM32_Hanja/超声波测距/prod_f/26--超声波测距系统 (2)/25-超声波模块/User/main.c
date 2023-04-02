#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CSB_SR04.h"
#include "Key.h"

void LED_UI(void)
{
	OLED_ShowChinese(4,11,0);  //ºÎ
	OLED_ShowChinese(4,13,1);  //¼Î
	OLED_ShowChinese(4,15,2);  //»Ô
	OLED_ShowChinese(2,1,3);  //¾à
	OLED_ShowChinese(2,3,4);  //Àë
	OLED_ShowChinese(2,13,5);  //Ã×
	OLED_ShowString(2,6,":  .");
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	LED_UI();
	CSB_SR04_Init();
	Key_Init();
	
	uint16_t con = 0, Knum;
	while (1)
	{
		Knum = Key_GetNum();
		con = Distance();
		OLED_ShowNum(2, 8, con/100, 1);
		OLED_ShowNum(2, 10, con%100, 2);
		
		OLED_ShowNum(3, 13,Knum, 2);
		Delay_ms(200);
	}
}
