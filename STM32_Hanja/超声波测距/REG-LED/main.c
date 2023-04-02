#include "stm32f10x.h"

int main(void){
	unsigned int i;
	// GPIOA clk
	*(unsigned int *)0x40021018 |= (1<<2);
	
	// IO: output
	*(unsigned int *)0x40010804 |= ((1)<<(4*0));
	
	// ODR register
	while(1){
	*(unsigned int *)0x4001080C &=~(1<<8);
	for(i=0;i<1000000;i++);
	*(unsigned int *)0x4001080C |=(1<<8);
	for(i=0;i<1000000;i++);

	}
	//-------------
//	// GPIOB clk
//	*(unsigned int *)0x40021018 |= (1<<3);
//	
//	// IO: output
//	*(unsigned int *)0x40010800 |= ((1)<<(4*0));
//	
//	// ODR register
//	*(unsigned int *)0x4001080C &=~(1<<0);
}

void SystemInit(void)
{
	
}

