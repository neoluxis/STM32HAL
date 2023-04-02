#ifndef __ADC__H__
#define __ADC_H__


#define NON                0xFF
#define TURN_LEFT_EV			 0x01  //��
#define TURN_RIGHT_EV      0x04  //��
#define TURN_DOWN_EV       0x02  //��
#define TURN_UP_EV         0x03  //��
#define RESTART_EV         0x05  //��λ

void ADC_Configuration(void); //����ADC�ɼ�
unsigned char JoyState(void); //��ȡҡ��״̬

#endif
