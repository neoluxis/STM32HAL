#ifndef __TIMER_H__
#define __TIMER_H__
#include "stm32f10x.h"

extern void TIME_Configuration(void);
extern int If_TimeOut(uint32_t* oldtick,uint32_t diff_tick);
extern void Tick_Updata(uint32_t* tick);
extern void Delay_ms(uint32_t ms);

#endif
