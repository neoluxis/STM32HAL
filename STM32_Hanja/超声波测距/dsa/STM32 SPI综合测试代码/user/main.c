
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "TFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "stdio.h"

int b = 0;
int c = 0;
double a = 0;

/*串口输出*/
int fputc(int ch, FILE*fp)
{
	if(fp == stdout)
	{
		if(ch == '\n')
		{
			while((USART1->SR & USART_SR_TXE) == 0);
			USART1->DR = '\r';
		}
		while((USART1->SR & USART_SR_TXE) == 0);
			USART1->DR = ch;
		
	}
	return 0;
	
}


void hcsr04_init()
{	
	GPIOA->ODR |= GPIO_ODR_ODR1;
	delaySS(20);
	GPIOA->ODR &=~ GPIO_ODR_ODR1;
}



int main(void)
{

  SystemInit();   // System init.
  delay_init(72); // Delay init.		
	Lcd_Init();
	Lcd_Clear(BLACK);
			RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN;
		RCC->APB1ENR = RCC_APB1ENR_TIM2EN;
		GPIOA->CRH = (GPIOA->CRH & 0xffff000f) | 0x34b0;
		GPIOA->CRL = (GPIOA->CRL & 0xffffff00) | 0x34;
		USART1->BRR = 72000000 / 9600;
		USART1->CR1 = USART_CR1_RE | USART_CR1_UE | USART_CR1_TE;
		printf("******************stm32f103rct6 HC-SR04**************\n");
		AFIO->EXTICR[2] |= AFIO_EXTICR1_EXTI0_PA; 
		EXTI->IMR |= EXTI_IMR_MR0; 
		EXTI->RTSR |= EXTI_RTSR_TR0;
		EXTI->FTSR |= EXTI_FTSR_TR0;
		TIM2->ARR = 65535;
		TIM2->PSC = 71;
		TIM2->EGR = TIM_EGR_UG;
		TIM2->CR1 =  TIM_CR1_CEN;
		NVIC_SetPriority(EXTI0_IRQn, 1);
		NVIC_EnableIRQ(EXTI0_IRQn);
  	Lcd_Init();
	Lcd_Clear(BLACK);
  while (1){
		hcsr04_init();
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

/*
***********************外部中断****************
*/
void EXTI0_IRQHandler(void)
{int i;

	if(GPIOA->IDR& GPIO_IDR_IDR0)
	{
		TIM2->CR1 |=  TIM_CR1_CEN;
	}
	else	
	{	
		TIM2->CR1 &=~  TIM_CR1_CEN; 
		TIM2->EGR = TIM_EGR_UG;

		b=  TIM2->CNT;
		b=b*170/10000;
		printf("%dcm   \n",b);
		delay_ms(1500);
	}
		EXTI->PR = EXTI_PR_PR0;
}
