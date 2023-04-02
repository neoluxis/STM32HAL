#include "USART1.h"

 void USART1_Configuration(void)//打印输出串口初始化
 {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
    
	  //配置串口1 （USART1） 时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	  //配置串口1接收终端的优先级
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	 
    //配置串口1 发送引脚(PA.09)
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);    
	  //配置串口1 接收引脚 (PA.10)
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	  //串口1工作模式（USART1 mode）配置 
	  USART_InitStructure.USART_BaudRate = 115200;//一般设置为9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位为8个字节
	  USART_InitStructure.USART_StopBits = USART_StopBits_1; //一位停止位
	  USART_InitStructure.USART_Parity = USART_Parity_No ; //无校验位
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //不需要流控制
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收发送模式
	  USART_Init(USART1, &USART_InitStructure); 
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	  USART_Cmd(USART1, ENABLE);//使能串口
}

int fputc(int ch, FILE *f) //重定向c库里面的fputc到串口，那么使用printf时就能将打印的信息从串口发送出去，在PC上同串口助手接收信息
{
	//将Printf内容发往串口
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

