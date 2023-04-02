#include "adc.h"
#include "stm32f10x.h"
//              ------------------------以下为摇杆所用到的引脚--------------------------------------
//              GND    		电源地
//              VCC   		接5V或3.3v电源
//              VRX       PA.2
//              VRY       PA.1
//              SW    		PA.0 

volatile u16 ADC_ConvertedValue[10][3];
void Adc_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
	/*使能GPIO和ADC1通道时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  
 
	/*将PA0设置为模拟输入*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	/*将GPIO设置为模拟输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/*将GPIO设置为模拟输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	/*将GPIO设置为模拟输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure; 
	
	Adc_GPIO_Config();	
        /*72M/6=12,ADC最大时间不能超过14M*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	
	/*将外设 ADC1 的全部寄存器重设为默认值*/
	ADC_DeInit(ADC1); 
        /*ADC工作模式:ADC1和ADC2工作在独立模式*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	/*模数转换工作在单通道模式*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	
	/*模数转换工作在单次转换模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
        /*ADC转换由软件而不是外部触发启动*/	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	/*ADC数据右对齐*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	/*顺序进行规则转换的ADC通道的数目*/
	ADC_InitStructure.ADC_NbrOfChannel = 3;	
	/*根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器*/   
	ADC_Init(ADC1, &ADC_InitStructure);	
 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
        /*使能指定的ADC1*/
	ADC_Cmd(ADC1, ENABLE);	
	ADC_DMACmd(ADC1, ENABLE);

	/*重置指定的ADC1的校准寄存器*/
	ADC_ResetCalibration(ADC1);	
	/*获取ADC1重置校准寄存器的状态,设置状态则等待*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*开始指定ADC1的校准*/
	ADC_StartCalibration(ADC1);		
        /*获取指定ADC1的校准程序,设置状态则等待*/
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

void get_Average(void)//取平均值
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
    说明：采用的12位adc转换器，那么最大的转换值是0xFFF，对应的电压最大3.3v；
最小转换值是0，对应的电压为0v；所以电压值跟转换值的关系就很明显了，当然这里不
需要算出电压值。middle_x、middle_y、idle_press的值是摇杆静态的检测值，我特意
打印出来，可能不同的原件静态值有区别。它们的值大概是在0~0xFFF中间，上下摇动
控制一个电位器，左右摇动控制一个电位器，所以在静态的时候是检测到中间值是合理
的，然后判断检测值相对于静态检测值的的变化趋势可得知摇动的方向。
*********************************************************************/

unsigned char JoyState(void)
{
    uint8_t temp1,temp2;
	  uint8_t event=NON;
	
    get_Average();
	
    /*计算X坐标的偏移*/
    temp1=(ConvertedValue[2]&0xF00)>>8;
    if(temp1<7){ //向左
			event=TURN_LEFT_EV;
    }else if(temp1>8){ //向右
			event=TURN_RIGHT_EV;
    }
    /*计算Y坐标的偏移*/
    temp2=(ConvertedValue[1]&0xF00)>>8;
    if(temp2<7){ //向上
        event=TURN_UP_EV;
    }else if(temp2>8){ //向下
				event=TURN_DOWN_EV;
    }
    /*判断SW按键的状态*/	
    if(ConvertedValue[0]<0x100){
        event=RESTART_EV;
    }
    return event;
}
