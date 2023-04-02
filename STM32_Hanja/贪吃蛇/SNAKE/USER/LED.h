#ifndef __LED_H_
#define __LED_H_

#define LED0_On() GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LED0_Off() GPIO_SetBits(GPIOA, GPIO_Pin_8)

void LED_Init();
void LED0_Toggle();

#endif
