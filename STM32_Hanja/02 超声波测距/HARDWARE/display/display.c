#include "display.h"
#include "oled.h"
#include  "delay.h"

void display(u8 adiszheng,u8 adisxiao,u8 ddisz,u8 ddisx,u8 empty)
{
		static u8 wei;

    //OLED_Refresh();
		if(wei==3&&empty!=100)
		OLED_Clear();
		
		if(wei==2&&empty<10)
		OLED_Clear();
		

		OLED_Show_scjl(0,16,0,16,1);//实
		OLED_Show_scjl(16,16,1,16,1);//测
		OLED_Show_scjl(32,16,2,16,1);//矩
		OLED_Show_scjl(48,16,3,16,1);//离
		
		OLED_Show_bjjl(0,32,0,16,1);//报
		OLED_Show_bjjl(16,32,1,16,1);//警
		OLED_Show_bjjl(32,32,2,16,1);//矩
		OLED_Show_bjjl(48,32,3,16,1);//离
		
		OLED_Show_zhankongbi(0,48,0,16,1);//占
		OLED_Show_zhankongbi(16,48,1,16,1);//空
		OLED_Show_zhankongbi(32,48,2,16,1);//比
		
		OLED_ShowNum(65,16,adiszheng,1,16,1);//实际距离整数
    OLED_ShowNum(80,16,adisxiao,2,16,1);//实际距离小数
		OLED_DrawPoint(75,27,1);//小数点
		OLED_Show_mi(95,16,0,16,1);//米
		
    OLED_ShowNum(65,32,ddisz,1,16,1);//报警距离整数
		OLED_ShowNum(80,32,ddisx,2,16,1);//报警距离整数
		OLED_DrawPoint(75,43,1);//小数点
		OLED_Show_mi(95,32,0,16,1);//米
		
    if(empty==100) wei=3;
    if(empty>=10&&empty<=99) wei=2;
    if(empty<=9) wei=1;
	
    OLED_ShowNum(55,48,empty,wei,16,1); 
	
		OLED_ShowString(80,48,"%",16,1);//%
    
    OLED_Refresh();
}
