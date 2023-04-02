#include "timer.h"
#include "led.h"

void TIM1_PWM_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_8);
  
  TIM_TimeBaseInitStruct.TIM_Period=arr;
  TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
  TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
  
  TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
  TIM_OCInitStruct.TIM_Pulse=0;//���ô�װ�벶��ȽϼĴ���������ֵ
  TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
  TIM_OC1Init(TIM1,&TIM_OCInitStruct);
  
  TIM_CtrlPWMOutputs(TIM1,ENABLE);//MOE�����ʹ��	
  
  TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
  
  TIM_Cmd(TIM1,ENABLE);
  
}

TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 //PA0 ����
	
	//��ʼ����ʱ��2 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM2���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
}


u8  TIM3CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH3_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 
 	if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{	    
			if(TIM3CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH3_CAPTURE_VAL=0XFFFF;
				}else TIM3CH3_CAPTURE_STA++;
			}	 
		}
	  if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
				TIM3CH3_CAPTURE_VAL=TIM_GetCapture1(TIM3);
		   	TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
      else  								//��δ��ʼ,��һ�β���������
			{
				TIM3CH3_CAPTURE_STA=0;			//���
				TIM3CH3_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM3,0);
				TIM3CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
		   	TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ	
}
