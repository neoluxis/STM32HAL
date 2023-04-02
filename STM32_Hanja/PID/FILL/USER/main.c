#include "main.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "motor.h"
#include "pwm.h"
#include "encoder.h"
#include "timer.h"

int targetVelocityL=800;
int currentVelocityL=1;

void updateData(int cv, int tv);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 
	
	// Inits
	LED_Init();
	delay_init();
	uart_init(9600);
	OLED_Init();
	OLED_ShowFrame();
	Motor_DirInit();
	Motor_SetDir(NOPWR);
	TIM1_PWM_Init(7199,0); // 10KHz
//	Encoder_CAP_Init();
	TIM6_Init(99,7199);
	
	while (1)
	{
		updateData(currentVelocityL, targetVelocityL);
	}
}

void updateData(int cv, int tv){
	OLED_ShowNum(32,0,cv,8,16);
	OLED_ShowNum(32,16,tv,8,16);
	printf("\r\n%u",cv);
}


