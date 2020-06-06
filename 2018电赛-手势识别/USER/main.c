#include "includes.h"
#include "Keyboard.h"

#define	Out_SET  	GPIOB->BSRR = GPIO_Pin_10  
#define	Out_CLR  	GPIOB->BRR  = GPIO_Pin_10 
signed int num = 0;
void OutIOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Out_CLR;
	
}
void Init()
{
	delay_init();
	Key_Init();	
	OutIOInit();
	Lcd_Init();
}

void Send_Start(void)
{
		Out_CLR;
		delay_ms(100);
		Out_SET;
		delay_ms(100);
		Out_CLR;
		delay_ms(100);
	
}
void Send_Num(unsigned int num1)
{
  signed int i = 0, j = 0, value[10],orin_num = num1;
	Send_Start();
	while(orin_num != 0)
	{
		value[i] = orin_num % 10 ;
		orin_num = orin_num /10;
		i ++;
		
	}
	while(j < i)
	{
		Out_SET;
		if(value[j] == 0)
			delay_ms(10*60+30);
		else if(value[j] <= 4)
		{
			delay_ms(value[j]*60+25);
		}
		else
			delay_ms(value[j]*60+45);
		Out_CLR;
		delay_ms(100);
		j++;
	}
	delay_ms(100);
}

int Keyboard_Input()
{
	signed int temp;
	temp  = Key_Scan();
	if(temp != -1)  
	{ 
		if(temp < 10 && temp >=0)
		    num = num *10 + temp;
		while(Key_Scan()!=-1);
	}
	LCD_P8x16Value(60,10,BLUE,WHITE,num);
	if(Key_Scan() == 14) 
	{
//		LCD_P6x8Str(10,60,BLUE,WHITE,"                                    ");
		delay_ms(1000);	
		LCD_P6x8Str(10,30,BLUE,WHITE,"Start to send ...     :"); 
	
		Send_Num(num);
		
		LCD_P6x8Str(10,30,BLUE,WHITE,"Successfully send ...:");   		
		while(Key_Scan()!=-1);
	}
	else if(Key_Scan() == 13)
	{
		num = 0;
		while(Key_Scan()!=-1);
	}
}
int main()
{
	u8 i = 0,j;
	signed char temp;
	
	Init();
	Lcd_Clear(WHITE);
	LCD_P8x16Str(10,10,BLUE,WHITE,"Send:");    
  LCD_ClearImage(25,50);
	while(1)
	{
		Keyboard_Input();
	}
	
	return 0;
}












