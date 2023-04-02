#include "USART1.h"

 void USART1_Configuration(void)//��ӡ������ڳ�ʼ��
 {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
    
	  //���ô���1 ��USART1�� ʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	  //���ô���1�����ն˵����ȼ�
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	 
    //���ô���1 ��������(PA.09)
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);    
	  //���ô���1 �������� (PA.10)
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	  //����1����ģʽ��USART1 mode������ 
	  USART_InitStructure.USART_BaudRate = 115200;//һ������Ϊ9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λΪ8���ֽ�
	  USART_InitStructure.USART_StopBits = USART_StopBits_1; //һλֹͣλ
	  USART_InitStructure.USART_Parity = USART_Parity_No ; //��У��λ
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ҫ������
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���շ���ģʽ
	  USART_Init(USART1, &USART_InitStructure); 
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	  USART_Cmd(USART1, ENABLE);//ʹ�ܴ���
}

int fputc(int ch, FILE *f) //�ض���c�������fputc�����ڣ���ôʹ��printfʱ���ܽ���ӡ����Ϣ�Ӵ��ڷ��ͳ�ȥ����PC��ͬ�������ֽ�����Ϣ
{
	//��Printf���ݷ�������
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

