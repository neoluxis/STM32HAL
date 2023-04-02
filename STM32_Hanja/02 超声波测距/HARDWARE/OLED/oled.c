#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 

u8 OLED_GRAM[128][8];

void SPI2_Init(void)           //硬件SPI2口初始化

{
  SPI_InitTypeDef  SPI_InitStructure;
  
	GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI2 and GPIOB clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE) ;//使能SPI2的时钟
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能IO口时钟
  
  /*  初始化SPI接口的: SCK, MOSI 引脚*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* PB12为片选，PB14为复位控制引脚*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);//复位信号设置为低
	GPIO_SetBits(GPIOB, GPIO_Pin_14);  //复位信号设置为高
  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//片选信号为低
  
  /* SPI2 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为单线发送模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	             //设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	         //SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                //时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;               //数据捕获于第二个时钟沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	                 //内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                //数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;                          //SPI_CRCPolynomial定义了用于CRC值计算的多项式
  SPI_Init(SPI2, &SPI_InitStructure);
  
  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
    
}


/*************************************************
函数功能：硬件SPI口发送或者接收一个字节数据

*************************************************/

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
unsigned char SPI2_ReadWriteByte(unsigned char TxData)
{		
	unsigned int retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>2000)return 0;
	}			  
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>2000)return 0;
	}	  						    
	return 0;          //返回收到的数据				    
}


//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd)
{	
	//unsigned char  i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();	//命令模式或者数据模式	  
	  OLED_CS_Clr();//片选
	
	//添加硬件读写SPI函数
	
	SPI2_ReadWriteByte(dat);
	 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}

//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 i,m,temp,csize,chr1;
	u8 x0=x,y0=y;
	if(size==8) csize=6;
	else csize=(size/8+((size%8)?1:0))*(size/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<csize;i++)
	{
		if(size==8)temp=asc2_0806[chr1][i];//调用0806字体
		else if(size==12)temp=asc2_1206[chr1][i];//调用1206字体
		else if(size==16)temp=asc2_1608[chr1][i];//调用1608字体
		else if(size==24)temp=asc2_2412[chr1][i];//调用2412字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x08)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size!=8)&&((x-x0)==size/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}

//m^n函数
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

void OLED_Show_scjl(u8 x,u8 y,u8 num,u8 size,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,csize=(size/8+((size%8)?1:0))*size;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<csize;i++)
	{
		temp=Hzk6[num][i];
	
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

void OLED_Show_bjjl(u8 x,u8 y,u8 num,u8 size,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,csize=(size/8+((size%8)?1:0))*size;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<csize;i++)
	{
		temp=Hzk7[num][i];
	
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

void OLED_Show_zhankongbi(u8 x,u8 y,u8 num,u8 size,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,csize=(size/8+((size%8)?1:0))*size;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<csize;i++)
	{
		temp=Hzk8[num][i];
	
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

void OLED_Show_mi(u8 x,u8 y,u8 num,u8 size,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,csize=(size/8+((size%8)?1:0))*size;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<csize;i++)
	{
		temp=Hzk9[num][i];
	
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{ 	

 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC,端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	           //RS脚接到PC6
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	             //RS脚接到PC6

  SPI2_Init();           //硬件SPI2口初始化							  
//	OLED_RST_Clr();
//	for(i=0;i<0xfff0;i++);//循环延时
//	OLED_RST_Set(); 
					  

	OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 

	OLED_Clear();
	
}  
