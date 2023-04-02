#include "oled.h"
#include "stm32f10x.h"
//              ------------------------以下为OLED显示所用到的引脚--------------------------------------
//              GND    		电源地
//              VCC   		接5V或3.3v电源
//              D0(CLK)   PA5（CLK）
//              D1(MOSI)  PA7（DIN）
//              DC    		PB0
//              CS   			PB1
//              RES   		接3.3v电源，当接地时会导致复位   

volatile unsigned char LCD_cache[8][128]={0}; //64*128显存 每一bit代表一个像素
         
void delay_ms(int n_ms) //大概延时        
{
 int j,k;
 for(j=0;j<n_ms;j++)
 for(k=0;k<5500;k++);
}
void GPIO_Config(void)//初始化IO口
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE); 													     
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
									
  /*PB.0->DC引脚   PB.1->CS引脚*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  ;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
	/*PA.5->CLK引脚  PA.7->MOSI引脚*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		
}

/*写指令到LCD模块*/
void OLED_TX_Command(int data)  
{
	char i;
	lcd_dc(0);;;
	lcd_cs(0);
	for(i=0;i<8;i++)
   {lcd_sclk(0);;;
		
		if(data&0x80) {lcd_sid(1);;;}
		else {lcd_sid(0);;;}
		lcd_sclk(1);
		__nop();;;
	//	lcd_sclk(0);;;
	 	data<<=1;
   }
	 	lcd_dc(1);;;
	 lcd_cs(1);
}

/*写数据到LCD模块*/
void OLED_TX_Data(int data)
{
	char i;
	lcd_dc(1);;;
	lcd_cs(0);
	for(i=0;i<8;i++)
   {
		lcd_sclk(0);;;
		if(data&0x80) {lcd_sid(1);;;}
		else {lcd_sid(0);;;}
		lcd_sclk(1);;;
		__nop();;;
		//lcd_sclk(0);;;
	 	data<<=1;
   }lcd_dc(1);;;
	 lcd_cs(1);
}


void OLED_Init(void) 
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB	, ENABLE );	  

	delay_ms(800);
	GPIO_Config();
	lcd_cs(0); //片选引脚激活
	
	OLED_TX_Command(0xAE);   //display off
	OLED_TX_Command(0x20);	//Set Memory Addressing Mode	
	OLED_TX_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_TX_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_TX_Command(0xc8);	//Set COM Output Scan Direction
	OLED_TX_Command(0x00);//---set low column address
	OLED_TX_Command(0x10);//---set high column address
	OLED_TX_Command(0x40);//--set start line address
	OLED_TX_Command(0x81);//--set contrast control register
	OLED_TX_Command(0xFF);
	OLED_TX_Command(0xa1);//--set segment re-map 0 to 127
	OLED_TX_Command(0xa6);//--set normal display
	OLED_TX_Command(0xa8);//--set multiplex ratio(1 to 64)
	OLED_TX_Command(0x3F);//
	OLED_TX_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_TX_Command(0xd3);//-set display offset
	OLED_TX_Command(0x00);//-not offset
	OLED_TX_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_TX_Command(0xf0);//--set divide ratio
	OLED_TX_Command(0xd9);//--set pre-charge period
	OLED_TX_Command(0x22);//
	OLED_TX_Command(0xda);//--set com pins hardware configuration
	OLED_TX_Command(0x12);
	OLED_TX_Command(0xdb);//--set vcomh
	OLED_TX_Command(0x20);//0x20,0.77xVcc
	OLED_TX_Command(0x8d);//--set DC-DC enable
	OLED_TX_Command(0x14);//
	OLED_TX_Command(0xaf);//--turn on oled panel 
	
	g.GPIO_Mode = GPIO_Mode_Out_PP;
	g.GPIO_Pin = GPIO_Pin_10;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &g);

	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	
}
void LCD_DrawPoint(uint8_t x,uint8_t y,uint8_t color)  //描点
{
	uint8_t temp,indx,page;

	if(color>1) color=1;
	indx=y%8; //indx=0
	page=y/8;
	
	temp=LCD_cache[page][x];
  temp &= ~(0x01<<indx);  //clean bit
  temp |= color<<indx;    //write bit
	LCD_cache[page][x]=temp;
}

void GUI_DrawChar(uint16_t x0,uint16_t y0,unsigned char ch) //在特定的位置显示一个字符
{
    uint8_t xcnt,ycnt,color,count=0;
    const uint8_t* pData;

    pData=&GUI_Font8x8ASCII_Data[8*(ch-0x20)];
	
    
	  for(xcnt=0;xcnt<8;xcnt++){
				count=0;
        for(ycnt=0;ycnt<8;ycnt++){
            color=((*pData)&(0x80>>count))>0?1:0;
            LCD_DrawPoint(y0+ycnt,63-(x0+xcnt),color);
            count++;
        }
        pData++;
    }
}
void OLED_DispString(int x0,int y0,char *str) //在指定的位置显示字符串
{
	while(*str){
		GUI_DrawChar(x0,y0,*str);
		str++;
		x0+=6;
		if(x0>=64){
			x0=0;
			y0+=10;
		}
	}
	
}

void OLED_ShowInt(int x0, int y0, int i){
	// to string
	char str[3] = {"000"};
	int in=2;
	while(i>0){
		str[in] = i%10 + 48;
		i/=10;
		in--;
	}
	OLED_DispString(x0, y0, str);
	return;
}

void OLED_Updata(void) //将LCD_canche显存里的数据发送到olcd进行显示
{
	unsigned char i,j;
	
	lcd_cs(0);
	for(i=0;i<8;i++)
	{
		OLED_TX_Command(0xb0+i);
		OLED_TX_Command(0x00);
		OLED_TX_Command(0x10);
		for(j=0;j<128;j++)
		{
		  	OLED_TX_Data(LCD_cache[i][j]);
		}
	}
 	lcd_cs(1);
}
