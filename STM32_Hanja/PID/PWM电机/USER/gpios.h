#ifndef _GPIOS_H
#define _GPIOS_H

#define IN1H()	GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define IN2H()	GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define IN1L()	GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define IN2L()	GPIO_ResetBits(GPIOA, GPIO_Pin_2)

void LN_Init();
void Motor_Init(void);

#endif
