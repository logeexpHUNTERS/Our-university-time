#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "timer.h"
#include "misc.h"

uint32_t usCnt   = 0;
uint32_t msCnt   = 0;
uint32_t us5_Cnt = 0;

extern int time;

void EXTI_TIMxOFF_ON(TIM_TypeDef *TIMx,TIMxSwitch State)
{
  if(State) 
  {
    us5_Cnt = 0;
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
    usCnt = 0;
    msCnt = 0;
  }
  else
  {
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE );
  }
}


void TIMxOFF_ON(TIM_TypeDef *TIMx,TIMxSwitch State)
{
  if(State)
    TIMx->CR1 |= 0x01;
  else if(!State)
    TIMx->CR1 &= ~(0x01);
}

void TIM3_Init(u16 arr,u16 psc)     // 中断频率 : 72M/[(arr+1)*(psc+1)]
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    
    TIM_TimeBaseStructure.TIM_Period        = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	  TIM_TimeBaseStructure.TIM_Prescaler     = psc; //设置用来作为TIMx时钟频率除数的预分频值
	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //TIM向上计数模式
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);
  
    TIM_Cmd(TIM3, ENABLE);  //使能TIM6 
}


void TIM4_Init(u16 arr,u16 psc)     // 中断频率 : 72M/[(arr+1)*(psc+1)]
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
    
    TIM_TimeBaseStructure.TIM_Period        = arr; 
	  TIM_TimeBaseStructure.TIM_Prescaler     = psc; 
	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  
	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
    TIM_Cmd(TIM4, ENABLE);  //使能TIM6 
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);     //清除TIMx更新中断标志 
	}
	usCnt++; 
  us5_Cnt++;
  if(usCnt == 333) // ms
  {
    msCnt++;   
    usCnt = 0;
  }
  
  if(msCnt == 1000)  // s
  {
    time++;
    msCnt = 0;
  }
}


void TIM4_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     //清除TIMx更新中断标志 
	}
	 
  
}

