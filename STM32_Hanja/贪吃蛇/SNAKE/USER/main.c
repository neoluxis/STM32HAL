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
	  USART1_Configuration(); //��ʼ������
	LED_Init();
	  TIME_Configuration(); //��ʼ����ʱ��
	  ADC_Configuration(); //��ʼ��ҡ�˵�ad�ɼ�
	  OLED_Init(); //oled��ʼ��Ĭ�ϲ���
		ReadData(&high);
    GameReady(); //������Ϸ׼������
	  Tick_Updata(&sysms);
    while (1){
		  event=JoyState(); //��ȡҡ�˵�״̬
			printDir(event);
			ret=Game_InputHandle(event);//���ƶ�
			if(ret){ //ret=1 ��ʾ��Ļ��ˢ��
				GameMapToLcdCache(); //�����ش���Ϸͼ��gamemapλͼ�����Ŵ��д��LcdCache�Դ�
				OLED_Updata();//��LcdCache[8][128]�Դ�����ݷ��͵���Ļ������ʾ
				sprintf(str,"S=%d, H=%d",GameScore, high);
				OLED_DispString(0,0,str); //��Ļ�Ϸ���ʾ����
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


