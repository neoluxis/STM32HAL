#include "stm32f10x.h"
#include "USART1.h"
#include "oled.h"
#include "Snake.h"
#include "Timer.h"
#include "adc.h"
#include "Menu.h"
#include "store.h"
#include "LED.h"

unsigned int sysms;
unsigned char event=0,ret;
char str[20]={0};

void printDir(int i){
				switch(event){
				case TURN_DOWN_EV:
					printf("Down");
					break;
				case TURN_LEFT_EV:
					printf("LEFT");
					break;
				case TURN_RIGHT_EV:
					printf("RIGHT");
					break;
				case TURN_UP_EV:
					printf("UP");
					break;
				case RESTART_EV:
					printf("RES");
					break;
				case NON:
					printf("NON");
					break;
			}
			printf("\n");
		}

int main(void)
{
	int o = 0;
	  RCC_PCLK1Config(RCC_HCLK_Div4);
	  USART1_Configuration(); //初始化串口
	LED_Init();
	  TIME_Configuration(); //初始化定时器
	  ADC_Configuration(); //初始化摇杆的ad采集
	  OLED_Init(); //oled初始化默认参数
		ReadData(&high);
    GameReady(); //进入游戏准备界面
	  Tick_Updata(&sysms);
    while (1){
		  event=JoyState(); //获取摇杆的状态
			printDir(event);
			ret=Game_InputHandle(event);//蛇移动
			if(ret){ //ret=1 表示屏幕需刷新
				GameMapToLcdCache(); //将像素从游戏图像gamemap位图经过放大后写进LcdCache显存
				OLED_Updata();//将LcdCache[8][128]显存的数据发送到屏幕进行显示
				sprintf(str,"S=%d, H=%d",GameScore, high);
				OLED_DispString(0,0,str); //屏幕上方显示分数
			}
			
			// Ruun correctly.
			if(o==250){
				LED0_Toggle();
				o=0;
			}else{
				o++;
			}
		}
}


