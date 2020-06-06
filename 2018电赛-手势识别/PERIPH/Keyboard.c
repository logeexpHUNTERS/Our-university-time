#include "Keyboard.h"
#include "oled.h"

/*---------------��ֵ��----------------------------------
				1		2		3		10    |      1     2    3     Mode 1
				4		5		6		11    |      4     5    6     Mode 2   
				7		8	  9		12		|	  	 7     8    9     Mode 3
				14	0		15	13    |     Exit   0 Confirm  Mode 4
		
	(�����ӿ�PF1~PF8 �ް�������ʱ���� -1)
-------------------------------------------------------*/ 

void Key_Init(void) //��ʼ���������Ҫʹ�õ�GPIO�ڡ�
{

 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //����PF1��PF4Ϊ�������������
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;       //����PF5��PF8Ϊ�������롣
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    //��Ϊ���涨������Ϊ���ʱ���Ѿ�������GPIOA��ʱ���ˣ�
	//���Դ˴�������Ҫ����RCC_APB2PeriphClockCmd()����ʱ���ˡ�
  GPIO_Init(GPIOA,&GPIO_InitStructure);		
}

int8_t Key_Scan(void) //ʵ�־�����̡�����ֵΪ���������ļ�ֵ���˼�ֵ���û��Լ����塣
{
	uint8_t KeyVal;	 //keyValΪ��󷵻صļ�ֵ��
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x000f)); //����PA0��PA3ȫ������ߡ�
	
	if((GPIOA->IDR & 0x00f0)==0x0000)  //���PF5��PF8ȫΪ0����û�м����¡���ʱ������ֵΪ-1.
		return -1;
	else
	{	
	    delay_ms(5);    //��ʱ5msȥ������
	    if((GPIOA->IDR & 0x00f0)==0x0000)//�����ʱ5ms��PF5��PF8��ȫΪ0���򣬸ղ����ŵĵ�λ�仯�Ƕ���������.
	    return -1;
		//	while((GPIOA->IDR & 0x00f0)!=0x0000);   //�ȴ���������
	}

	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x0001));	//��PF4��PF1��������Ƶ�0001.

		switch(GPIOA->IDR & 0x00f0)//��PF5��PF8��ֵ�����жϣ��������ͬ�ļ�ֵ��
			{
				case 0x0010: KeyVal=13; break;
				case 0x0020: KeyVal=0;	break;
				case 0x0040: KeyVal=14;	break;
				case 0x0080: KeyVal=15;	break;
			}
	   
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x0002));	//��PF4��PF1��������Ƶ�0.
		switch(GPIOA->IDR & 0x00f0)		        //��PF5��PF8��ֵ�����жϣ��������ͬ�ļ�ֵ��
		{
			case 0x0010: KeyVal=7;	break;
			case 0x0020: KeyVal=8;	break;
			case 0x0040: KeyVal=9;	break;
			case 0x0080: KeyVal=12;	break;
		}

	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x0004));	//��PF4��PF1��������Ƶ�1011.
		switch(GPIOA->IDR & 0x00f0)		        //��PF5��PF8��ֵ�����жϣ��������ͬ�ļ�ֵ��
		{
			case 0x0010: KeyVal=4;	break;
			case 0x0020: KeyVal=5;	break;
			case 0x0040: KeyVal=6;	break;
			case 0x0080: KeyVal=11;	break;
		}
 
	 GPIO_Write(GPIOA,(GPIOA->ODR & 0xfff0 | 0x008));	//��PF4��F1��������Ƶ�0111.
		switch(GPIOA->IDR & 0x00f0)		        //��PF5��PF8��ֵ�����жϣ��������ͬ�ļ�ֵ��
		{
			case 0x0010: KeyVal=1;	break; //
			case 0x0020: KeyVal=2;	break;
			case 0x0040: KeyVal=3;	break;
			case 0x0080: KeyVal=10;	break;
		}									  
	return KeyVal;		
}

/*******************************************************************************
* �� �� ��         : Key_Input
* ��������		   : ͨ����������2~3����������ȷ�ϼ��������
* ��    ��         : ��
* ��    ��         : ����ֵ�������ֵ��
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

