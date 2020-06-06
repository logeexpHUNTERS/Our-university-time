#include "timer.h"
#include "TIM_Int.h"



/********TIM2中断服务函数************/

void TIM2_IRQHandler(void)  
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}


/*********TIM4中断服务函数************/

void TIM4_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}




/***********其它引脚配置**********/

void Pin_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_OutInitStructure;
 GPIO_InitTypeDef  GPIO_InInitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //使能PB
	
	
 GPIO_OutInitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13; //模式选择引脚；水中/空中 
 GPIO_OutInitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		     //推挽输出
 GPIO_OutInitStructure.GPIO_Speed = GPIO_Speed_50MHz;		       //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_OutInitStructure);		
						 
	
 GPIO_InInitStructure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_15;	   
 GPIO_InInitStructure.GPIO_Mode  = GPIO_Mode_IPD;                //PB14/15上拉输入
 GPIO_Init(GPIOB, &GPIO_InInitStructure);
	
	
}

