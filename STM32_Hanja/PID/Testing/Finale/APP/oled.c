//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;

//********************************************************************************
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
//#include "delay.h"

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
unsigned char  OLED_GRAM[128][8];	 

//�����Դ浽LCD		
void OLED_Refresh_Gram(void)
{
	unsigned char  i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd)
{	
	//unsigned char  i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();	//����ģʽ��������ģʽ	  
	  OLED_CS_Clr();//Ƭѡ
	
	//����Ӳ����дSPI����
	
	SPI2_ReadWriteByte(dat);
	 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	unsigned char  i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}
//���������
void LCD_Set_Pos(unsigned char  x, unsigned char  y) 
{ 
		OLED_WR_Byte(0xb0+(7-y/8),OLED_CMD);
		OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x&0x0f),OLED_CMD); 
} 
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
{
	unsigned char  pos,bx,temp=0;
		LCD_Set_Pos(x, y);  
		if(x>127||y>64)return;//������Χ��.
		pos=7-y/8;
		bx=y%8;
		temp=1<<(7-bx);
		if(t)OLED_GRAM[x][pos]|=temp;
		else OLED_GRAM[x][pos]&=~temp;	
	  
     //������ʾλ�á��иߵ�ַ   
	  OLED_WR_Byte(OLED_GRAM[x][pos],OLED_DATA); 
	
  //	OLED_WR_Byte(OLED_GRAM[x][pos],OLED_DATA); 
	  

}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot)  
{  
	unsigned char  x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //����1206����
		else temp=oled_asc2_1608[chr][t];		 //����1608���� 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  no)//��ʾ����
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	unsigned char  mode=1;
	//chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
    for(t=0;t<32;t++)
    {   
	
			temp= CHINESE[no][t];		 //����1608���� 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
      }          
}  
//m^n����
unsigned int oled_pow(unsigned char  m,unsigned char  n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size)
{         	
	unsigned char  t,temp;
	unsigned char  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p)
{
#define MAX_CHAR_POSX 127
#define MAX_CHAR_POSY 63         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   

void OLED_ShowDouble(unsigned char  numX, unsigned char  numY, double d){
	int integer=0, decimal=0, decimals[2]={0};
	integer = (int)(d/1);
	decimal = 100*(d - integer);
	decimals[0] = decimal / 10;
	decimals[1] = decimal - 10*decimals[0];
	OLED_ShowNum(numX, numY, integer,1,16);
	OLED_ShowChar(numX+8,numY,'.',16,1);
	OLED_ShowInteger(numX, numY, decimal);
}

void OLED_ShowInteger(unsigned char  numX, unsigned char  numY, int i){
	int digits[2] = {0};
	if(i>=99) i=99;
	digits[0] = i / 10;
	digits[1] = i - (i/10)*10;
	OLED_ShowNum(numX+16, numY, digits[0],1,16);
	OLED_ShowNum(numX+24, numY, digits[1],1,16);
}

void OLED_ShowFrame(void){
// Curretn Velo
	OLED_ShowString(5, 3, "C V: ");

// Target Velo
	OLED_ShowString(5, 20, "T V: ");
}

void updateData(double measured, double alert, int onwork){
	unsigned int Kanji_width = 16, numX=1, numY=1;
	numX = 1 + 5*Kanji_width;
	numY = 1;
	OLED_ShowDouble(numX, numY, measured);
	numY += Kanji_width;
	OLED_ShowDouble(numX, numY, alert);
	numY+=Kanji_width;
	OLED_ShowInteger(numX, numY, onwork);
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 //A0�Žӵ�OLED��RST�� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //A0�Žӵ�OLED��RST�� 

  							  
//	OLED_RST_Clr();
//	delay_ms(100);
//	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}  




























