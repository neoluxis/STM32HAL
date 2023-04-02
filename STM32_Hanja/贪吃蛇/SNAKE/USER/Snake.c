#include "Snake.h"
#include "stdio.h"
#include "string.h"
#include "Timer.h"
#include "oled.h"
#include "adc.h"
#include "stmflash.h"
#include "store.h"

unsigned int speed_turn=0,speed_restart=0,speed_move=0,speed_max=200;
unsigned char gamemap[HEIGHT][WIDTH]={0}; //游戏图像的像素缓存
int GameScore=0,GameStatus=GAMEPAUSE;
int high=0;

SNAKELIST SnokeNodeBuff[HEIGHT*WIDTH];//先开辟一些空节点，因为不能用malloc
int NodeBuffIndex=0,Snokedirection=TURN_UP_EV; //NodeBuffIndex记录所用的节点下标
SNAKELIST SnokeHeadNode;//链表头节点

void GAME_SnakeDestroyInGameMap(unsigned char (*gamemap)[WIDTH],SNAKELIST* SnakeList)//去掉蛇尾，移动前做得工作
{
	SNAKELIST *n=SnakeList->prev;
	
	gamemap[n->y][n->x]=0;
}

void GAME_SnakeFillInGameMap(unsigned char (*gamemap)[WIDTH],SNAKELIST* SnakeList)//将蛇图像填入游戏图像缓存
{
	SNAKELIST *n=SnakeList;
	
	while(n->next!=NULL){
		gamemap[n->y][n->x]=1;
		n=n->next;
	}
}

void GAME_SnakeListAddNode(SNAKELIST* SnakeList,int x,int y) //后部添加节点，吃食物时给链表添加节点
{
	SNAKELIST *h=SnakeList,*p=SnakeList->prev;
	
	SnokeNodeBuff[NodeBuffIndex].x=x;
	SnokeNodeBuff[NodeBuffIndex].y=y;
	SnokeNodeBuff[NodeBuffIndex].next=NULL;
	SnokeNodeBuff[NodeBuffIndex].prev=p;
	
	h->prev=&SnokeNodeBuff[NodeBuffIndex];//头节点的前继节点指向自身
	p->next=&SnokeNodeBuff[NodeBuffIndex];//原尾巴节点的后继节点指向自身
	
	NodeBuffIndex++;//指向SnokeNodeBuff下一个未用节点，为下一次做准备
}
void GAME_BackgroundInit(unsigned char (*gamemap)[WIDTH]) //游戏背景初始化
{
	int i,j;
	
	memset(gamemap,0,HEIGHT*WIDTH);
	
	for(i=0;i<WIDTH;i++){ //上下边框
		gamemap[0][i]=1;
		gamemap[HEIGHT-1][i]=1;
	}
	for(i=0;i<HEIGHT;i++){ //左右边框
		gamemap[i][0]=1;
		gamemap[i][WIDTH-1]=1;
	}
}

void GAME_NewSnake(SNAKELIST* SnakeList) //初始化链表，初始化蛇长度，4+1节
{
	int x=4,y=15,i;//蛇的初始位置
	
	SnakeList->x=x;
	SnakeList->y=y++;
	SnakeList->prev=SnakeList;
	SnakeList->next=SnakeList;
	
	for(i=0;i<4;i++){
		GAME_SnakeListAddNode(SnakeList,x,y+i);
	}
	GAME_SnakeFillInGameMap(gamemap,SnakeList);
}

void GAME_NewFood(unsigned char (*gamemap)[WIDTH]) //在地图上随机产生新的食物
{
	unsigned int seed1,seed2;
	int x,y;
	
	while(1){
		Tick_Updata(&seed1);
		Tick_Updata(&seed2);
		x=seed1%WIDTH;
		y=seed2%HEIGHT;
		if(gamemap[y][x]==0){
			gamemap[y][x]=2;
			break;
		}
	}
}


unsigned char GAME_SnakeMove(unsigned char (*gamemap)[WIDTH],SNAKELIST* SnakeList,int dir,unsigned int *gamescore) // 蛇移动，dir=方向
{
	SNAKELIST *p,*h;
	unsigned char val,ret=1;

	GAME_SnakeDestroyInGameMap(gamemap,SnakeList); //去掉蛇尾，移动前做得工作
	
	h=SnakeList;
	p=SnakeList->prev;//指向前继节点，头节点的前继节点是尾巴
	
	//蛇移动
	while(p!=SnakeList){//从最后一个节点开始，后继结点的x、y等于前继节点的x、y，直到头节点为止,头节点的x、y先不变
		p->x=p->prev->x; //等于该节点的前继节点的x
		p->y=p->prev->y; //等于该节点的前继节点的y
		p=p->prev;
	}
	switch(dir){
		case TURN_UP_EV: //上
			h->y--;
			break;
		case TURN_DOWN_EV: //下
			h->y++;
			break;
		case TURN_LEFT_EV: //左
			h->x--;
			break;
		case TURN_RIGHT_EV: //右
			h->x++;
			break;
		default:
			break;
	}
	val=gamemap[h->y][h->x]; //蛇头将要经过的点
	
	switch(val){
		case 0: //可以移动
			break;
		case 1: //碰到障碍物或者蛇身
			GameStatus=GAMEPAUSE;
			GameOver();
			printf("游戏结束！\r\n");
			ret=0;//游戏失败
			break;
		case 2: //吃到食物
			GAME_SnakeListAddNode(SnakeList,h->x,h->y);
		  GAME_NewFood(gamemap);
		  (*gamescore)++;
			high = high>GameScore?high:GameScore;
			WriteData(high);
			break;
		default:
			break;
	}

	GAME_SnakeFillInGameMap(gamemap,SnakeList);
	return ret;
}

#define SPPED_RESTART_MAX 1000  //1
#define SPPED_MOVE        500   //蛇自动前进一步的周期500ms

unsigned char Game_InputHandle(unsigned char event)//对输入按键事件的处理
{
	unsigned char ret=0;
	int times = 0;
	
	if(GameStatus==GAMEPAUSE&&event!=RESTART_EV){ //游戏状态未运行时，除非按下restart，否则不进入
		return 0;
	}
	
	if(event==5-Snokedirection){//按下蛇前进的相反方向时，忽略
		event=NON; 
	}
	
	switch(event){
		case NON:
			speed_max=200;
			if(If_TimeOut(&speed_move,SPPED_MOVE)){ //自动前进
					Tick_Updata(&speed_move);
			    event=Snokedirection;
		  }else{
				break;
			}
		case TURN_LEFT_EV: //蛇向左移动
		case TURN_RIGHT_EV://蛇向右移动
		case TURN_DOWN_EV:	//蛇向下移动	
		case TURN_UP_EV:		//蛇向上移动	
			if(If_TimeOut(&speed_turn,speed_max)){
				Tick_Updata(&speed_turn);
				speed_max=150;
				Snokedirection=event;
				ret=GAME_SnakeMove(gamemap,&SnokeHeadNode,event,&GameScore);	
				printf("event=%d\r\n",event);
			}
			break;
		case RESTART_EV://游戏复位
			if(If_TimeOut(&speed_restart,SPPED_RESTART_MAX)){
					Tick_Updata(&speed_restart);
					GameStatus=GAMERUNING;
					GameScore=0;
				  Snokedirection=TURN_UP_EV;
					GAME_BackgroundInit(gamemap);
					GAME_NewSnake(&SnokeHeadNode);
					GAME_NewFood(gamemap);
					ret=1;
					printf("event=%d\r\n",event);
			}
		  break;
		default:
			//error
			break;
	}
	return ret;
}

extern volatile unsigned char LCD_cache[8][128];//引用非本文件的变量

void GameMapToLcdCache(void) //对游戏图像（16*30）进行放大然后填入lcd（64*128）的显存，游戏界面图像的一个像素转变成lcd的4*4个像素
{
	int i,j,gm_x=15,gm_y=0;
	
	for(i=0;i<16;i++){
		gm_y=0;
		for(j=8;j<128;j++){ //行映射
			if((j%4==0)&&(j!=8)){
				gm_y++;
			}
			if(gamemap[gm_y][gm_x]){
					if(i%2==0){
						LCD_cache[i/2][j] |= 0x0F; //低四位
					}else{
						LCD_cache[i/2][j] |= 0xF0; //高四位
					}
			}else{
					if(i%2==0){
						LCD_cache[i/2][j] &= 0xF0; //低四位
					}else{
						LCD_cache[i/2][j] &= 0x0F; //高四位
					}
			}
		}
    gm_x--;
	}
}

#include "LED.h"
void GameReady(void) //游戏初始界面，等待restart按键按下开始游戏
{
	static int o=0;
	memset(LCD_cache,0,sizeof(LCD_cache));
	OLED_DispString(0,30,"Please");
	OLED_DispString(0,40,"press the");
	OLED_DispString(0,50,"reset key");
	OLED_DispString(0,60,"to start.");
	OLED_DispString(5,80,"000--");
	OLED_ShowInt(30,80,high);
	OLED_Updata();
	if(o==1000){
				LED0_Toggle();
				o=0;
			}else{
				o++;
			}
}
void GameOver(void) //游戏接收界面，按下restart按键继续游戏
{
	memset(LCD_cache,0,sizeof(LCD_cache));
	OLED_DispString(0,50,"GAME OVER");
	OLED_DispString(0,70,"Scr: ");
	OLED_ShowInt(25,70, GameScore);
	OLED_DispString(0,80, "Hst: ");
	OLED_ShowInt(25, 80, high);
	OLED_Updata();
}
