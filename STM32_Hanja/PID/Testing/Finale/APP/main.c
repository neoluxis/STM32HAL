/**
  ******************************************************************************
  * @file    Project/Template/main.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

unsigned char *P_RXD;//接收数据指针

unsigned int Num_RXD=0;//要打印字节区位码的字节数


unsigned char TxBuffer[256]={0,2,3,};//串口发送缓冲区

unsigned char RxBuffer[256]; //串口接收缓冲区

unsigned char Key1_Value=1;

unsigned char Key2_Value=1;

unsigned char t;

int main(void)
{
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  RCC_Configuration();

  NVIC_Configuration();

  GPIO_Configuration();

  SysTick_init();		     //延时初始化

  SPI2_Init(); 			     //初始化SPI硬件口

	P_RXD=RxBuffer;//接收指针指向接收缓冲区
	
  USART_Configuration();  //USART1配置 

  TIM2_Config();			//定时器初始化 

  OLED_Init();			 //初始化OLED      
	OLED_ShowFrame();
	
  /* Infinite loop */
  while (1)
  {
		printf("\r\n你a%d", 1); //
		delay_ms(100);
		
  }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/**
  * @}
  */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
