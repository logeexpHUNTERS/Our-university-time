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

void TIM3_Init(u16 arr,u16 psc)     // �ж�Ƶ�� : 72M/[(arr+1)*(psc+1)]
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    
    TIM_TimeBaseStructure.TIM_Period        = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	  TIM_TimeBaseStructure.TIM_Prescaler     = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);
  
    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM6 
}


void TIM4_Init(u16 arr,u16 psc)     // �ж�Ƶ�� : 72M/[(arr+1)*(psc+1)]
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
  
    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM6 
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);     //���TIMx�����жϱ�־ 
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


void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     //���TIMx�����жϱ�־ 
	}
	 
  
}

