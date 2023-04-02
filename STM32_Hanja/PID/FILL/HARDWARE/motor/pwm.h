#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f10X.h"

void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);

#endif

