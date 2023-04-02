#include "adc.h"
#include "stm32f10x.h"
//              ------------------------����Ϊҡ�����õ�������--------------------------------------
//              GND    		��Դ��
//              VCC   		��5V��3.3v��Դ
//              VRX       PA.2
//              VRY       PA.1
//              SW    		PA.0 

volatile u16 ADC_ConvertedValue[10][3];
void Adc_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
	/*ʹ��GPIO��ADC1ͨ��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  
 
	/*��PA0����Ϊģ������*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	/*��GPIO����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/*��GPIO����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	/*��GPIO����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure; 
	
	Adc_GPIO_Config();	
        /*72M/6=12,ADC���ʱ�䲻�ܳ���14M*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	
	/*������ ADC1 ��ȫ���Ĵ�������ΪĬ��ֵ*/
	ADC_DeInit(ADC1); 
        /*ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	/*ģ��ת�������ڵ�ͨ��ģʽ*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	
	/*ģ��ת�������ڵ���ת��ģʽ*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
        /*ADCת��������������ⲿ��������*/	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	/*ADC�����Ҷ���*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	/*˳����й���ת����ADCͨ������Ŀ*/
	ADC_InitStructure.ADC_NbrOfChannel = 3;	
	/*����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���*/   
	ADC_Init(ADC1, &ADC_InitStructure);	
 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
        /*ʹ��ָ����ADC1*/
	ADC_Cmd(ADC1, ENABLE);	
	ADC_DMACmd(ADC1, ENABLE);

	/*����ָ����ADC1��У׼�Ĵ���*/
	ADC_ResetCalibration(ADC1);	
	/*��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*��ʼָ��ADC1��У׼*/
	ADC_StartCalibration(ADC1);		
        /*��ȡָ��ADC1��У׼����,����״̬��ȴ�*/
	while(ADC_GetCalibrationStatus(ADC1));		
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR); 
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&ADC_ConvertedValue; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = 30; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
		
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16 ConvertedValue[3];

void get_Average(void)//ȡƽ��ֵ
{
    u8 i,j;
    int sum;
	
    for(i=0;i<3;i++){
        sum=0;
        for(j=0;j<10;j++){	
            sum+=ADC_ConvertedValue[j][i];
        }
        ConvertedValue[i]=sum/10;
    }
}
#define middle_x  0x07e2
#define middle_y  0x0813
#define idle_press 0x07cd
/*******************************************************************
    ˵�������õ�12λadcת��������ô����ת��ֵ��0xFFF����Ӧ�ĵ�ѹ���3.3v��
��Сת��ֵ��0����Ӧ�ĵ�ѹΪ0v�����Ե�ѹֵ��ת��ֵ�Ĺ�ϵ�ͺ������ˣ���Ȼ���ﲻ
��Ҫ�����ѹֵ��middle_x��middle_y��idle_press��ֵ��ҡ�˾�̬�ļ��ֵ��������
��ӡ���������ܲ�ͬ��ԭ����ֵ̬���������ǵ�ֵ�������0~0xFFF�м䣬����ҡ��
����һ����λ��������ҡ������һ����λ���������ھ�̬��ʱ���Ǽ�⵽�м�ֵ�Ǻ���
�ģ�Ȼ���жϼ��ֵ����ھ�̬���ֵ�ĵı仯���ƿɵ�֪ҡ���ķ���
*********************************************************************/

unsigned char JoyState(void)
{
    uint8_t temp1,temp2;
	  uint8_t event=NON;
	
    get_Average();
	
    /*����X�����ƫ��*/
    temp1=(ConvertedValue[2]&0xF00)>>8;
    if(temp1<7){ //����
			event=TURN_LEFT_EV;
    }else if(temp1>8){ //����
			event=TURN_RIGHT_EV;
    }
    /*����Y�����ƫ��*/
    temp2=(ConvertedValue[1]&0xF00)>>8;
    if(temp2<7){ //����
        event=TURN_UP_EV;
    }else if(temp2>8){ //����
				event=TURN_DOWN_EV;
    }
    /*�ж�SW������״̬*/	
    if(ConvertedValue[0]<0x100){
        event=RESTART_EV;
    }
    return event;
}
