#ifndef __ADC__H__
#define __ADC_H__


#define NON                0xFF
#define TURN_LEFT_EV			 0x01  //左
#define TURN_RIGHT_EV      0x04  //右
#define TURN_DOWN_EV       0x02  //下
#define TURN_UP_EV         0x03  //上
#define RESTART_EV         0x05  //复位

void ADC_Configuration(void); //配置ADC采集
unsigned char JoyState(void); //获取摇杆状态

#endif
