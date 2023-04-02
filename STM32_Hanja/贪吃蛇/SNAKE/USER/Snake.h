#ifndef __TETRIS__H__
#define __TETRIS__H__


#define  HEIGHT  30
#define  WIDTH   16
#define GAMERUNING   0x00
#define GAMEPAUSE		 0x01

#define NULL 0x00

typedef struct SnakeNode{
	int x;
	int y;
	struct SnakeNode *next,*prev;
}SNAKELIST;

void GAME_Init(void);
unsigned char Game_InputHandle(unsigned char event);
void GameMapToLcdCache(void);
void GameReady(void);
void GameOver(void);

extern int GameStatus;
extern int GameScore;
extern int high;
#endif
