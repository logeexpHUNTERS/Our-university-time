#include "Keyboard.h"
#include "oled.h"

/*---------------键值表----------------------------------
				1		2		3		10    |      1     2    3     Mode 1
				4		5		6		11    |      4     5    6     Mode 2   
				7		8	  9		12		|	  	 7     8    9     Mode 3
				14	0		15	13    |     Exit   0 Confirm  Mode 4
		
	(按键接口PF1~PF8 无按键按下时返回 -1)
-------------------------------------------------------*/ 

void Key_Init(void) //初始化矩阵键盘要使用的GPIO口。
{

 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //定义PF1到PF4为推挽输出、、。
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;       //定义PF5到PF8为下拉输入。
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    //因为上面定义引脚为输出时，已经打开整个GPIOA的时钟了，
	//所以此处不再需要函数RCC_APB2PeriphClockCmd()来打开时钟了。
  GPIO_Init(GPIOA,&GPIO_InitStructure);		
}

int8_t Key_Scan(void) //实现矩阵键盘。返回值为，各按键的键值，此键值由用户自己定义。
{
	uint8_t KeyVal;	 //keyVal为最后返回的键值。
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x000f)); //先让PA0到PA3全部输出高。
	
	if((GPIOA->IDR & 0x00f0)==0x0000)  //如果PF5到PF8全为0，则没有键按下。此时，返回值为-1.
		return -1;
	else
	{	
	    delay_ms(5);    //延时5ms去抖动。
	    if((GPIOA->IDR & 0x00f0)==0x0000)//如果延时5ms后，PF5到PF8又全为0，则，刚才引脚的电位变化是抖动产生的.
	    return -1;
		//	while((GPIOA->IDR & 0x00f0)!=0x0000);   //等待按键松手
	}

	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x0001));	//让PF4到PF1输出二进制的0001.

		switch(GPIOA->IDR & 0x00f0)//对PF5到PF8的值进行判断，以输出不同的键值。
			{
				case 0x0010: KeyVal=13; break;
				case 0x0020: KeyVal=0;	break;
				case 0x0040: KeyVal=14;	break;
				case 0x0080: KeyVal=15;	break;
			}
	   
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x0002));	//让PF4到PF1输出二进制的0.
		switch(GPIOA->IDR & 0x00f0)		        //对PF5到PF8的值进行判断，以输出不同的键值。
		{
			case 0x0010: KeyVal=7;	break;
			case 0x0020: KeyVal=8;	break;
			case 0x0040: KeyVal=9;	break;
			case 0x0080: KeyVal=12;	break;
		}

	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x0004));	//让PF4到PF1输出二进制的1011.
		switch(GPIOA->IDR & 0x00f0)		        //对PF5到PF8的值进行判断，以输出不同的键值。
		{
			case 0x0010: KeyVal=4;	break;
			case 0x0020: KeyVal=5;	break;
			case 0x0040: KeyVal=6;	break;
			case 0x0080: KeyVal=11;	break;
		}
 
	 GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x008));	//让PF4到F1输出二进制的0111.
		switch(GPIOA->IDR & 0x00f0)		        //对PF5到PF8的值进行判断，以输出不同的键值。
		{
			case 0x0010: KeyVal=1;	break; //
			case 0x0020: KeyVal=2;	break;
			case 0x0040: KeyVal=3;	break;
			case 0x0080: KeyVal=10;	break;
		}									  
	return KeyVal;		
}

/*******************************************************************************
* 函 数 名         : Key_Input
* 函数功能		   : 通过键盘输入2~3个数，并按确认键完成输入
* 输    入         : 无
* 输    出         : 返回值（键入的值）
*******************************************************************************/
int16_t Key_Input(void)
{
	int8_t Key[3]={-1,-1,-1},Key_sum=0,i=4;
	int8_t temp_val = -1;
	while(--i)
	{	
		while((temp_val = Key_Scan())== -1 ||((temp_val>10 && temp_val <15 )));
		delay_ms(10);
	//	OLED_P6x8Value(64,5,temp_val);
		if(temp_val==15)	break;
		Key[i]=temp_val;
		Key_sum=Key_sum*10+Key[i];
		//OLED_P6x8Value(64,5,Key_sum);
		while(Key_Scan()!= -1);
	}
	return Key_sum;
}

