/*
 * key.c
 *
 *  Created on: 2023年1月29日
 *      Author: Administrator
 */

#include "key.h"
#include "main.h"

int KeyScan() {
    int key = 0;
    if (!HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) ||
    !HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) ||
            HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin)){
        HAL_Delay(10);
        if (!HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)){
            key = KEY0_PRES;
        }else if (!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)){
            key = KEY1_PRES;
        }else if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin)){
            key = WK_UP_PRES;
        }
    }
    return key;
}
