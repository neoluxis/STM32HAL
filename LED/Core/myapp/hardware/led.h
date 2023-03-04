/*
 * led.h
 *
 *  Created on: Jan 29, 2023
 *      Author: Administrator
 */

#ifndef MYAPP_HARDWARE_LED_H_
#define MYAPP_HARDWARE_LED_H_

#include "main.h"

#define LED0_ON()  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_OFF()  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#define LED0_INV() HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)
#define LED1_ON() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_INV()  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)

#endif /* MYAPP_HARDWARE_LED_H_ */
