#include "store.h"
#include "stmflash.h"

void ReadData(int*hi){
	u16 t = 0, l=0, it=0;
	char ss[3] = {"000"};
	u8 wei[3] = {0},i=0;;
	STMFLASH_Read(SCOREADDR, (u16*)ss, 3);
	for(it=0;ss[it]!=0;it++){
		l++;
	}
	for(i = 0; i < 3; i++)
	{
		switch(ss[i])
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
	switch(l){
		case 1:
			t=wei[0];
			break;
		case 2:
			t = wei[0] * 10 + wei[1];
			break;
		case 3:
			t = wei[0]*100 + wei[1]*10 +wei[2];
			break;
		default:
			t=0;
			break;
	}
//	t = wei[0]*1 + wei[1]*10 + wei[2]*100;
	if(t <= 400){
		*hi = t;
	}else{
		*hi=0;
	}
}
	
void WriteData(int hi){
	u16 t=hi;
	char ss[3];
	sprintf(ss, "%d", t);
	STMFLASH_Write(SCOREADDR, (u16*)ss, 3);
}

