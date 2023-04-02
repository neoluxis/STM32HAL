#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"


//Mini STM32开发板范例代码2
//按键输入实验
//正点原子@ALIENTEK
//技术论坛:www.openedv.com	 

 int main(void)
 {
	u8 t;	
 	SystemInit();  //系统时钟设置
	delay_init(72);	     //延时初始化
	NVIC_Configuration();//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init();	//初始化与LED连接的硬件接口
	KEY_Init();	//初始化与按键连接的硬件接口


 }

