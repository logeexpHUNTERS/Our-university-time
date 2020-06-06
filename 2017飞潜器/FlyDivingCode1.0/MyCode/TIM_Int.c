#include "timer.h"
#include "TIM_Int.h"



/********TIM2�жϷ�����************/

void TIM2_IRQHandler(void)  
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}


/*********TIM4�жϷ�����************/

void TIM4_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}




/***********������������**********/

void Pin_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_OutInitStructure;
 GPIO_InitTypeDef  GPIO_InInitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //ʹ��PB
	
	
 GPIO_OutInitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13; //ģʽѡ�����ţ�ˮ��/���� 
 GPIO_OutInitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		     //�������
 GPIO_OutInitStructure.GPIO_Speed = GPIO_Speed_50MHz;		       //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_OutInitStructure);		
						 
	
 GPIO_InInitStructure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_15;	   
 GPIO_InInitStructure.GPIO_Mode  = GPIO_Mode_IPD;                //PB14/15��������
 GPIO_Init(GPIOB, &GPIO_InInitStructure);
	
	
}

