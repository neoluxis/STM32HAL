#include "stm32f10x.h"                  // Device header
#include "stdio.h"

#define e_I_max 50
#define Output_max 50
#define Output_min (-50)
#define Kp_Init 11
#define Ki_Init (0.5)
#define Kd_Init (0.8)

    struct PID_struct
    {
        float Kp;
        float Ki;
        float Kd;
        float Output;    
        float e;
        float e_I;
        float e_last;
    };
    
    struct PID_struct my_PID=
    {
        .Kp=Kp_Init,
        .Ki=Ki_Init,
        .Kd=Kd_Init,
        .Output=0,
        .e=0,
        .e_I=0,
        .e_last=0
    
    }; 

int16_t myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}


   
float PID(float Target,float Rate)
{
	Rate=myabs(Rate);
	my_PID.e=Target-Rate;
	my_PID.e_I+=my_PID.e;
	
	if(my_PID.e_I> e_I_max) my_PID.e_I=e_I_max;
	
	my_PID.Output=my_PID.Kp*my_PID.e+my_PID.Ki*my_PID.e_I+my_PID.Kd*(my_PID.e-my_PID.e_last);
	
	my_PID.e_last=my_PID.e;
	
	if(my_PID.Output> Output_max) my_PID.Output=Output_max;
	else if(my_PID.Output<Output_min) my_PID.Output=Output_min;
	
	return my_PID.Output;
}
    
    
