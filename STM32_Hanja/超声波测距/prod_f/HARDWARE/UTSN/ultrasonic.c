#include <stm32f10x.h>
#include "delay.h"
#include "stdio.h"
#include "ultrasonic.h"

void Ultrasonic_Init(){
	GPIOA->ODR |= GPIO_ODR_ODR1;
	 delay_us(20);
	GPIOA->ODR &=~ GPIO_ODR_ODR1;
}

void EXTI0_IRQHandler(void)
{

//	if(GPIOA->IDR& GPIO_IDR_IDR0)
//	{
//		TIM2->CR1 |=  TIM_CR1_CEN;
//	}
//	else	
//	{	
//		TIM2->CR1 &=~  TIM_CR1_CEN; 
//		TIM2->EGR = TIM_EGR_UG;

//		b =  TIM2->CNT;
//		b = b*170/10000;
//		printf("%dcm   ",b);
//		for(int i = 0; i < 3000000; i++);
//	}
//		EXTI->PR = EXTI_PR_PR0;
}

