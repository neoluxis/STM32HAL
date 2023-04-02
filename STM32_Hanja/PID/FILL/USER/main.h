#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

/**
 Include
*/

#include "led.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
#include "delay.h"

// global variables
// speed
extern int targetVelocityL;
extern int currentVelocityL;

// EXIT CODE
#define EXIT_SUCCCESS 0

#endif
