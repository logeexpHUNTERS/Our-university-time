#include "sys.h"
#include "usart.h"	  
#include "math.h" 
void uart_init(u32 bound){
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ���Լ����ù���ʱ��
	 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

 //Usart1 NVIC ����

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}


void USART1_PrintI(long int data,unsigned char newline)
{
	unsigned char temp[12], i = 0;
	if(data < 0)
	{
	   data = - data;
	   while((USART1->SR & 0X40) == 0);
	   USART1->DR = '-';                   //���͸���
	}
	if(data == 0)
	{
	   while((USART1->SR &0X40) == 0);  
	   USART1->DR = 48;
	} 
	else
	{
		while(data != 0)  
		{	
			i++;
			temp[i] = data % 10 + 48; //��������������λת��ΪASCII�뷢��
			data = data / 10;         
		}
		for( ; i > 0 ; i--)
		{
		   while((USART1->SR & 0X40) == 0);        //�ȴ��������
		   USART1->DR = temp[i];
		}
	}
	
	if(newline == 1)     //���η��� \r \n
	{
		while((USART1->SR & 0X40) == 0){};    USART1->DR = 13;    
		while((USART1->SR & 0X40) == 0){};    USART1->DR = 10	;   
	}
	
	 
}

void USART1_PrintF(double data,unsigned char precision,unsigned char newline)
{
	unsigned char temp[12], i = 0, j ;
	unsigned char Add_Flag = 0;

	if(data < 0)
	{
	   data = - data;
	   while((USART1->SR & 0X40) == 0);
	   USART1->DR = '-';                   //���͸���
		
	}
	if(data == 0)
	{
	   while((USART1->SR &0X40) == 0); 
	   USART1->DR = 48;
	}
	else
	{		
			for(j = 0; j < precision; j ++)
					data = data * 10;  
			if(data<pow(10,precision))
			{
				data = data + pow(10,precision);    // ������Ч������С���������λ����� -> ��λ + 1   ��ʾʱ�� - 1
				Add_Flag = 1;
			}				
			
			while((int)data != 0)  
			{	
				i++;
				temp[i] = (int)data % 10 + 48;               //��������������λת��ΪASCII�뷢��
				if( i == precision )    i ++ , temp[i] = '.';
				if( i == precision + 2 && Add_Flag == 1)   temp[i] = temp[i] - 1; 
				data = data / 10;              
			}
			
			for( ; i > 0 ; i--)
			{
				 while((USART1->SR & 0X40) == 0);        //�ȴ��������
				 USART1->DR = temp[i];
			}
		
	}
	if(newline == 1)     //���η��� \r \n
	{
		while((USART1->SR & 0X40) == 0){};    USART1->DR = 13;    
		while((USART1->SR & 0X40) == 0){};    USART1->DR = 10	;   
	}
		
}
unsigned char Res;
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
			  		 
  } 
}
