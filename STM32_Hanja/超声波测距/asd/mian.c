/*
****************************说明**********************
1.stm32f103rct6
2.hc-sr04 :PA0、PA1
3.串口：USART1
4.具体连线及讲解见博客：https://blog.csdn.net/sf9090/article/details/105151911
*******************************************************************************
*******************谢谢支持****************************************************
********************************作者：https://blog.csdn.net/sf9090************
*/

#include <stm32f10x.h>
#include <stdio.h>
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
/*
************延时函数***********
*/
void delay_us(int x)
{
	TIM1->ARR = x-1;
	TIM1->PSC = 71;
	TIM1->EGR = TIM_EGR_UG;
	TIM1->CR1 = TIM_CR1_OPM | TIM_CR1_CEN; 
	while(TIM1->CR1 & TIM_CR1_CEN);
}
/*初始化测距模块*/
void hcsr04_init()
{	
	GPIOA->ODR |= GPIO_ODR_ODR1;
	 delay_us(20);
	GPIOA->ODR &=~ GPIO_ODR_ODR1;
}

int main(void)
{
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN;
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;
	GPIOA->CRH = (GPIOA->CRH & 0xffff000f) | 0x34b0;
	GPIOA->CRL = (GPIOA->CRL & 0xffffff00) | 0x34;
	USART1->BRR = SystemCoreClock / 9600;
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
	
	
	while(1)
	{
		hcsr04_init();
	}
}
/*
***********************外部中断****************
*/
void EXTI0_IRQHandler(void)
{

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
		printf("%dcm   ",b);
		for(int i = 0; i < 3000000; i++);
	}
		EXTI->PR = EXTI_PR_PR0;
}
