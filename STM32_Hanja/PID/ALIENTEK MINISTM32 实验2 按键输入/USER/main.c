#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"


//Mini STM32�����巶������2
//��������ʵ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com	 

 int main(void)
 {
	u8 t;	
 	SystemInit();  //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init();	//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();	//��ʼ���밴�����ӵ�Ӳ���ӿ�


 }

