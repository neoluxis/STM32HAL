#ifndef __OLED__H__
#define __OLED__H__

#define ON  0
#define OFF 1
					
#define lcd_cs(a)	if (a)	\
					GPIOB->BSRR = GPIO_Pin_1;\
					else		\
					GPIOB->BRR = GPIO_Pin_1;

#define lcd_dc(a)	if (a)	\
					GPIOB->BSRR = GPIO_Pin_0;\
					else		\
					GPIOB->BRR = GPIO_Pin_0;
					
#define lcd_res(a)	if (a)	\
					GPIOA->BSRR = GPIO_Pin_12;\
					else		\
					GPIOA->BRR = GPIO_Pin_12;\
					
#define lcd_sid(a)	if (a)	\
	        GPIOA->BSRR = GPIO_Pin_7;\
					else		\
					GPIOA->BRR = GPIO_Pin_7;

#define lcd_sclk(a)	if (a)	\
	        GPIOA->BSRR = GPIO_Pin_5;\
					else		\
					GPIOA->BRR = GPIO_Pin_5;

extern const unsigned char GUI_Font8x8ASCII_Data[8*128];
					
/*写指令到LCD模块*/
void OLED_TX_Command(int data1);  
/*写数据到LCD模块*/
void OLED_TX_Data(int data1);  
					
void OLED_Init(void);
void OLED_Updata(void);
void OLED_Clear(void);			
					
void OLED_DispString(int x0,int y0,char *str);		
void OLED_ShowInt(int,int,int);					
#endif
