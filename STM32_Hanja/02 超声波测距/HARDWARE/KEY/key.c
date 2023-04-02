#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "sys.h"

void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode&&KEY0==0)  //֧�ֳ���
  {
    delay_ms(30);
    if(KEY0==0) return 1;
  }
      
	if(key_up&&KEY0==0) //֧�ֵ���
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0) return 2;
	}
  else if(KEY0==1) key_up=1; 	    
 	return 0;// �ް�������
}

u16 key_jia(u16 x)
{
  u16 x0=x;
  while(KEY_Scan(1)!=1)
  {
    if(KEY_Scan(0)==2&&x<185) x++;
  }
  if(KEY_Scan(0)==2) return x;
  return x0;
}

u16 key_jian(u16 x)
{
  u16 x0=x;
  while(KEY_Scan(1)!=1)
  {
    if(KEY_Scan(0)==2&&x>10) x--;
  }
  if(KEY_Scan(0)==2) return x;
  return x0;
}
