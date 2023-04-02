#include "stm32f10x.h"
#include "stmflash.h" 
#include "stdio.h"
#include "Delay.h"
#include "Oled.h"
#include "Bmp.h"
#include "AD.h"
#include "usart.h"

#define FLASH_SAVE_ADDR  0X08020000 

u32 snake_x[1000]={0};
u32 snake_y[1000]={0};
u32 teme = 2; //第teme组坐标
u32 length = 2;
u8 x1, y1;  //食物坐标
u8 x = 62, y = 30;  //初始坐标
u8 off = 0;  //界面切换
int high = 0;  //历史最高分
char number[] = {"0"};
char datatemp[2];

void num_change(void)
{
	u8 wei[2] = {0};
	
	for(u8 i = 0; i < 3; i++)
	{
		switch(datatemp[i])
		{
			case '0': wei[i] = 0; break;
			case '1': wei[i] = 1; break;
			case '2': wei[i] = 2; break;
			case '3': wei[i] = 3; break;
			case '4': wei[i] = 4; break;
			case '5': wei[i] = 5; break;
			case '6': wei[i] = 6; break;
			case '7': wei[i] = 7; break;
			case '8': wei[i] = 8; break;
			case '9': wei[i] = 9; break;
		}
	}
	
	high = wei[0]*10 + wei[1]*1;
}

void OLED_UI2(void)
{
	if(length > high)
	{
		high = length;
		sprintf(number, "%d", high);
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)number,2);
	}
	OLED_ShowString(20,0,"your score",16);
	OLED_ShowNum(55,20,length,2,12);
	OLED_ShowString(0,45,"alltime high",16);
	OLED_ShowNum(105,50,high,2,12);
	OLED_Refresh();
}

void snake_head(u8 i, u8 j)
{
	OLED_DrawPoint(i+1,j-1);
	OLED_DrawPoint(i+1,j+1);
	OLED_DrawPoint(i,j+1);
	OLED_DrawPoint(i+1,j);
	OLED_DrawPoint(i,j);
	OLED_DrawPoint(i-1,j);
	OLED_DrawPoint(i,j-1);
	OLED_DrawPoint(i-1,j+1);
	OLED_DrawPoint(i-1,j-1);	
	OLED_Refresh();
}

void snake_body(u8 i, u8 j)
{
	OLED_DrawPoint(i+1,j-1);
	OLED_DrawPoint(i+1,j+1);
	OLED_DrawPoint(i,j+1);
	OLED_DrawPoint(i+1,j);
	OLED_DrawPoint(i-1,j);
	OLED_DrawPoint(i,j-1);
	OLED_DrawPoint(i-1,j+1);
	OLED_DrawPoint(i-1,j-1);	
	OLED_Refresh();
}

void snake_first(void)
{
	snake_x[2] = x;
	snake_y[2] = y;
	snake_head(snake_x[2],snake_y[2]);
	
	snake_x[1] = x - 3;
	snake_y[1] = y;
	snake_body(snake_x[1],snake_y[1]);
	
	snake_x[0] = x - 6;
	snake_y[0] = y;
	snake_body(snake_x[0],snake_y[0]);
}

void snake_cleartali(u8 i, u8 j)
{
	OLED_ClearPoint(i+1,j-1);
	OLED_ClearPoint(i+1,j+1);
	OLED_ClearPoint(i,j+1);
	OLED_ClearPoint(i+1,j);
	OLED_ClearPoint(i-1,j);
	OLED_ClearPoint(i,j-1);
	OLED_ClearPoint(i-1,j+1);
	OLED_ClearPoint(i-1,j-1);	
	OLED_Refresh();	
}

void food(void)
{
	while(1)
	{
		x1=rand()%123;
		y1=rand()%60;
		
		for(u8 i = teme; i > teme - length; i--)
		{
			if(snake_x[i] == x1 || snake_y[i] == y1)
			{
				x1 = 0;
				break;
			}
		}
		
		if(x1%3==0 && x1>1 && y1%3==0 && y1>2)
		{
			break;
		}
	}
	x1 = x1 + 2;
	snake_head(x1,y1);
}

void OLED_UI(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,2);
	num_change();
	
	OLED_Refresh();
	OLED_ShowPicture(0,0,128,8,BMP2);
	Delay_ms(4000);
	OLED_Clear();
	
	OLED_ShowString(18,0,"alltime high",16);
	OLED_ShowNum(55,20,high,2,16);
	OLED_Refresh();
	Delay_ms(2500);
	OLED_Clear();
	
	OLED_DrawLine(0,1,127,1);
	OLED_DrawLine(0,62,127,62);
	OLED_DrawLine(0,1,0,62);
	OLED_DrawLine(127,1,127,63);
	OLED_Refresh();
	snake_first();
	food();
}


void eat_food(void)
{
	if(snake_x[teme] == x1 && snake_y[teme] == y1)
	{
		length++;
		food();
	}
}

void collide(void)
{
	if(snake_x[teme] >= 126 || snake_x[teme] <= 1 || snake_y[teme] >= 61 || snake_y[teme] < 3)
	{
		off = 2;
	}

	for(u8 i = teme - 1; i > teme - length; i--)
	{
		if(snake_x[i] == snake_x[teme] && snake_y[i] == snake_y[teme])
		{
			off = 2;
		}
	}
}

void snake_move(void)
{
	u8 direction = 3; //0上； 1下； 2左； 3右
	
	OLED_ShowNum(0,0,length,2,12);
	OLED_Refresh();
	
	if(AD_Value[1] > 4000 && direction != 1)	{direction = 0; printf("u");}
	if(AD_Value[1] < 10 && direction != 0)	    {direction = 1;printf("d");}
	if(AD_Value[0] > 4000 && direction != 3)	{direction = 2;printf("l");}
	if(AD_Value[0] < 10 && direction != 2)	    {direction = 3;printf("r");}
	
	
	if(direction == 0)
	{
		y -= 3;
		OLED_ClearPoint(snake_x[teme],snake_y[teme]);
		snake_cleartali(snake_x[teme - length],snake_y[teme - length]);
		teme++;
		snake_x[teme] = x;
		snake_y[teme] = y;
		snake_head(snake_x[teme],snake_y[teme]);
	}
	
	if(direction == 1)
	{
		y += 3;
		OLED_ClearPoint(snake_x[teme],snake_y[teme]);
		snake_cleartali(snake_x[teme - length],snake_y[teme - length]);
		teme++;
		snake_x[teme] = x;
		snake_y[teme] = y;
		snake_head(snake_x[teme],snake_y[teme]);
	}
	
	if(direction == 2)
	{
		x -= 3;
		OLED_ClearPoint(snake_x[teme],snake_y[teme]);
		snake_cleartali(snake_x[teme - length],snake_y[teme - length]);
		teme++;
		snake_x[teme] = x;
		snake_y[teme] = y;
		snake_head(snake_x[teme],snake_y[teme]);
	}
	
	if(direction == 3)
	{
		x += 3;
		OLED_ClearPoint(snake_x[teme],snake_y[teme]);
		snake_cleartali(snake_x[teme - length],snake_y[teme - length]);
		teme++;
		snake_x[teme] = x;
		snake_y[teme] = y;
		snake_head(snake_x[teme],snake_y[teme]);
	}
}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	AD_Init();
	OLED_Init();
	SystemInit();
	
	OLED_UI();	

	while(1)
	{
		if(AD_Value[0] < 10 && off == 0)	{off = 1;}
		
		if(off == 1)
		{
			snake_move();
			eat_food();
			collide();
			Delay_ms(50);			
		}
		
		if(off == 2)
		{
			Delay_ms(1500);
			OLED_Clear();
			off = 3;			
		}
		
		if(off == 3)
		{
			OLED_UI2();
		}
		
	}
}
