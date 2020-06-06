#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

uchar receive_data;

sbit led = P1^6;
//sbit led2 = P2^2;

void delay(uchar i);
void UartInit(void);

void main()
{	
	UartInit();
  while(1)
	{
	  if(receive_data == '0')
		{
			led = 1;
		}
		else
		{
			led = 0;
		}
	}
}


void delay(uchar i)//????
{
	while(i--);	
}


void UartInit(void)	
{
	 TMOD&=0x0F;    //
   TMOD|=0x20;    //
	 SCON=0x50; 
	 TH1=0xf4;      //256-FOSC/(BOUD*12*16);  
	 TL1=0xf4;      //256-FOSC/(BOUD*12*16);
	 PCON|=0x00;   
	 ES=1;         
	 TR1=1;        
	 REN=1;         
	 EA=1;
}




/*******************************************************************************
* ? ? ?       : ser() interrupt 4
* ????		 : ??????
* ?    ?       : 
* ?    ?    	 : 
*******************************************************************************/

void ser()interrupt 4
{
	 RI = 0;
	 receive_data = SBUF;
}