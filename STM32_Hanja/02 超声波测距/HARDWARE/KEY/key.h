#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

void KEY_Init(void);
u8 KEY_Scan(u8);
u16 key_jia(u16 x);
u16 key_jian(u16 x);


#endif
