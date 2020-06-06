#include "Syn.h"
#include "nrf24l01.h"

#define Equipment 1

#if Equipment      /****  接收  ****/

u8 exti2Flag       = '0'; // W1
u8 exti9_5Flag     = '0'; // W2
u8 exti15_10Flag   = '0'; // W3

extern int ti2Tim;
extern int ti9Tim;
extern int ti12Tim;

extern uint32_t usCnt;
extern uint32_t msCnt ;
extern uint32_t us5_Cnt;

void recPinInit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOD, ENABLE ); // W1
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE ); // W3 W2
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO,  ENABLE );
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;    // W2
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;   // W3
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;    // W1
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    EXTI_ClearITPendingBit(EXTI_Line12);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
    
    EXTI_ClearITPendingBit(EXTI_Line2);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
    
    EXTI_ClearITPendingBit(EXTI_Line9);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
    
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  
    
}



void EXTI2_IRQHandler(void)
{
  EXTILineOFF_ON(EXTI_Line2,EXTI_OFF);
  ti2Tim = us5_Cnt;
  exti2Flag = '1';
   
  EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI9_5_IRQHandler(void)
{
  EXTILineOFF_ON(EXTI_Line9,EXTI_OFF);
  ti9Tim = us5_Cnt;
  exti9_5Flag = '1';
  
  EXTI_ClearITPendingBit(EXTI_Line9);
}

void EXTI15_10_IRQHandler(void)
{
  EXTILineOFF_ON(EXTI_Line12,EXTI_OFF);
  ti12Tim = us5_Cnt;
  exti15_10Flag = '1';
  
  EXTI_ClearITPendingBit(EXTI_Line12);
}


#elif !Equipment  /****  发射 ****/

void SendPort_Init()  // PC8
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO, ENABLE );
  RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM3, ENABLE );
  
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse       = 100;
  
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
  
  TIM_Cmd(TIM3, ENABLE);  //使能TIM3 
  
  TIM_SetCompare3(TIM3, 10);
 
}

#endif





void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;       
	AFIO->MAPR&=0XF8FFFFFF; 
	AFIO->MAPR|=temp;       
} 

void EXTILineOFF_ON(uint32_t EXTI_Line,EXTIxLine State)
{
  
  if(State)
    EXTI->IMR |=   EXTI_Line;
  else if(!State)
	EXTI->IMR &= ~(EXTI_Line); 
}

