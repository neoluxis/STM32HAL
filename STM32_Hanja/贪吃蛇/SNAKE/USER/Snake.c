#include "Snake.h"
#include "stdio.h"
#include "string.h"
#include "Timer.h"
#include "oled.h"
#include "adc.h"
#include "stmflash.h"
#include "store.h"

unsigned int speed_turn=0,speed_restart=0,speed_move=0,speed_max=200;
unsigned char gamemap[HEIGHT][WIDTH]={0}; //��Ϸͼ������ػ���
int GameScore=0,GameStatus=GAMEPAUSE;
int high=0;

SNAKELIST SnokeNodeBuff[HEIGHT*WIDTH];//�ȿ���һЩ�սڵ㣬��Ϊ������malloc
int NodeBuffIndex=0,Snokedirection=TURN_UP_EV; //NodeBuffIndex��¼���õĽڵ��±�
SNAKELIST SnokeHeadNode;//����ͷ�ڵ�

void GAME_SnakeDestroyInGameMap(unsigned char (*gamemap)[WIDTH],SNAKELIST* SnakeList)//ȥ����β���ƶ�ǰ���ù���
{
	SNAKELIST *n=SnakeList->prev;
	
	gamemap[n->y][n->x]=0;
}

void GAME_SnakeFillInGameMap(unsigned char (*gamemap)[WIDTH],SNAKELIST* SnakeList)//����ͼ��������Ϸͼ�񻺴�
{
	SNAKELIST *n=SnakeList;
	
	while(n->next!=NULL){
		gamemap[n->y][n->x]=1;
		n=n->next;
	}
}

void GAME_SnakeListAddNode(SNAKELIST* SnakeList,int x,int y) //����ӽڵ㣬��ʳ��ʱ��������ӽڵ�
{
	SNAKELIST *h=SnakeList,*p=SnakeList->prev;
	
	SnokeNodeBuff[NodeBuffIndex].x=x;
	SnokeNodeBuff[NodeBuffIndex].y=y;
	SnokeNodeBuff[NodeBuffIndex].next=NULL;
	SnokeNodeBuff[NodeBuffIndex].prev=p;
	
	h->prev=&SnokeNodeBuff[NodeBuffIndex];//ͷ�ڵ��ǰ�̽ڵ�ָ������
	p->next=&SnokeNodeBuff[NodeBuffIndex];//ԭβ�ͽڵ�ĺ�̽ڵ�ָ������
	
	NodeBuffIndex++;//ָ��SnokeNodeBuff��һ��δ�ýڵ㣬Ϊ��һ����׼��
}
void GAME_BackgroundInit(unsigned char (*gamemap)[WIDTH]) //��Ϸ������ʼ��
{
	int i,j;
	
	memset(gamemap,0,HEIGHT*WIDTH);
	
	for(i=0;i<WIDTH;i++){ //���±߿�
		gamemap[0][i]=1;
		gamemap[HEIGHT-1][i]=1;
	}
	for(i=0;i<HEIGHT;i++){ //���ұ߿�
		gamemap[i][0]=1;
		gamemap[i][WIDTH-1]=1;
	}
}

void GAME_NewSnake(SNAKELIST* SnakeList) //��ʼ��������ʼ���߳��ȣ�4+1��
{
	int x=4,y=15,i;//�ߵĳ�ʼλ��
	
	SnakeList->x=x;
	SnakeList->y=y++;
	SnakeList->prev=SnakeList;
	SnakeList->next=SnakeList;
	
	for(i=0;i<4;i++){
		GAME_SnakeListAddNode(SnakeList,x,y+i);
	}
	GAME_SnakeFillInGameMap(gamemap,SnakeList);
}

void GAME_NewFood(unsigned char (*gamemap)[WIDTH]) //�ڵ�ͼ����������µ�ʳ��
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


unsigned char GAME_SnakeMove(unsigned char (*gamemap)[WIDTH],SNAKELIST* SnakeList,int dir,unsigned int *gamescore) // ���ƶ���dir=����
{
	SNAKELIST *p,*h;
	unsigned char val,ret=1;

	GAME_SnakeDestroyInGameMap(gamemap,SnakeList); //ȥ����β���ƶ�ǰ���ù���
	
	h=SnakeList;
	p=SnakeList->prev;//ָ��ǰ�̽ڵ㣬ͷ�ڵ��ǰ�̽ڵ���β��
	
	//���ƶ�
	while(p!=SnakeList){//�����һ���ڵ㿪ʼ����̽���x��y����ǰ�̽ڵ��x��y��ֱ��ͷ�ڵ�Ϊֹ,ͷ�ڵ��x��y�Ȳ���
		p->x=p->prev->x; //���ڸýڵ��ǰ�̽ڵ��x
		p->y=p->prev->y; //���ڸýڵ��ǰ�̽ڵ��y
		p=p->prev;
	}
	switch(dir){
		case TURN_UP_EV: //��
			h->y--;
			break;
		case TURN_DOWN_EV: //��
			h->y++;
			break;
		case TURN_LEFT_EV: //��
			h->x--;
			break;
		case TURN_RIGHT_EV: //��
			h->x++;
			break;
		default:
			break;
	}
	val=gamemap[h->y][h->x]; //��ͷ��Ҫ�����ĵ�
	
	switch(val){
		case 0: //�����ƶ�
			break;
		case 1: //�����ϰ����������
			GameStatus=GAMEPAUSE;
			GameOver();
			printf("��Ϸ������\r\n");
			ret=0;//��Ϸʧ��
			break;
		case 2: //�Ե�ʳ��
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
#define SPPED_MOVE        500   //���Զ�ǰ��һ��������500ms

unsigned char Game_InputHandle(unsigned char event)//�����밴���¼��Ĵ���
{
	unsigned char ret=0;
	int times = 0;
	
	if(GameStatus==GAMEPAUSE&&event!=RESTART_EV){ //��Ϸ״̬δ����ʱ�����ǰ���restart�����򲻽���
		return 0;
	}
	
	if(event==5-Snokedirection){//������ǰ�����෴����ʱ������
		event=NON; 
	}
	
	switch(event){
		case NON:
			speed_max=200;
			if(If_TimeOut(&speed_move,SPPED_MOVE)){ //�Զ�ǰ��
					Tick_Updata(&speed_move);
			    event=Snokedirection;
		  }else{
				break;
			}
		case TURN_LEFT_EV: //�������ƶ�
		case TURN_RIGHT_EV://�������ƶ�
		case TURN_DOWN_EV:	//�������ƶ�	
		case TURN_UP_EV:		//�������ƶ�	
			if(If_TimeOut(&speed_turn,speed_max)){
				Tick_Updata(&speed_turn);
				speed_max=150;
				Snokedirection=event;
				ret=GAME_SnakeMove(gamemap,&SnokeHeadNode,event,&GameScore);	
				printf("event=%d\r\n",event);
			}
			break;
		case RESTART_EV://��Ϸ��λ
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

extern volatile unsigned char LCD_cache[8][128];//���÷Ǳ��ļ��ı���

void GameMapToLcdCache(void) //����Ϸͼ��16*30�����зŴ�Ȼ������lcd��64*128�����Դ棬��Ϸ����ͼ���һ������ת���lcd��4*4������
{
	int i,j,gm_x=15,gm_y=0;
	
	for(i=0;i<16;i++){
		gm_y=0;
		for(j=8;j<128;j++){ //��ӳ��
			if((j%4==0)&&(j!=8)){
				gm_y++;
			}
			if(gamemap[gm_y][gm_x]){
					if(i%2==0){
						LCD_cache[i/2][j] |= 0x0F; //����λ
					}else{
						LCD_cache[i/2][j] |= 0xF0; //����λ
					}
			}else{
					if(i%2==0){
						LCD_cache[i/2][j] &= 0xF0; //����λ
					}else{
						LCD_cache[i/2][j] &= 0x0F; //����λ
					}
			}
		}
    gm_x--;
	}
}

#include "LED.h"
void GameReady(void) //��Ϸ��ʼ���棬�ȴ�restart�������¿�ʼ��Ϸ
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
void GameOver(void) //��Ϸ���ս��棬����restart����������Ϸ
{
	memset(LCD_cache,0,sizeof(LCD_cache));
	OLED_DispString(0,50,"GAME OVER");
	OLED_DispString(0,70,"Scr: ");
	OLED_ShowInt(25,70, GameScore);
	OLED_DispString(0,80, "Hst: ");
	OLED_ShowInt(25, 80, high);
	OLED_Updata();
}
