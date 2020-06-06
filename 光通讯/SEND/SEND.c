#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

sbit k1 = P2^1;
sbit LED = P2^2;

uchar Send_Table[10] = {'0','1','2','3','4','5','6','7','8','9'};

uchar sendFlag = 0;

void UartInit(void);
void Send(uchar Data);
void Delayms(uint xms);
void key_scan();

void main()
{
	//uchar i;
  UartInit();
	while(1)
	{
		 key_scan();
		 if(sendFlag == 1) LED = 1;
		 else LED = 0;
		
		 if(sendFlag == 0)
		 {
				Send(Send_Table[0]);
		 }
  }
}



void UartInit(void)		
{
   TMOD&=0x0F;    //???1??????4?
   TMOD|=0x20;    //???1
	 SCON=0x50; 
	 TH1=0xf4;      //256-FOSC/(BOUD*12*16);  
	 TL1=0xf4;      //256-FOSC/(BOUD*12*16);
	 PCON|=0x00;   
	 ES=1;         
	 TR1=1;        
	 REN=1;         
	 EA=1;
}

void Send(uchar Data)
{
	 SBUF = Data; 
 	 while(!TI);
	 TI = 0;
}


void key_scan()
{
	if(k1==0)		  //????K1????
	{
		Delayms(10);  //???? ????20ms
		if(k1==0)	   //??????????
		{
			while(k1 == 0);	 //????????
			sendFlag = 1;
		}
	}		
}

void Delayms(uint xms)		//@11.0592MHz
{
	unsigned char i, j;
  for(i=xms; i>0; i--)
		for(j=110;j>0;j--);
	
}
