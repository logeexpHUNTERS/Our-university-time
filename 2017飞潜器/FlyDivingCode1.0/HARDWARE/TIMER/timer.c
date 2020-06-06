#include "timer.h"
#include "led.h"
#include "usart.h"
#include "TIM_Int.h"
							  
//********************************************************************************
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��!






void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	
//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);     //��ӳ��

   //���ø�����Ϊ�����������,���TIM2 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1 PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 //���ø�����Ϊ�����������,���TIM2 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //TIM_CH2 PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//���ø�����Ϊ�����������,���TIM2 CH3��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH3 PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//���ø�����Ϊ�����������,���TIM2 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;         //TIM_CH4 PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	//��ʼ��TIM3 PWMģʽ
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;      //���ô�װ�벶��ȽϼĴ���������ֵ

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3   
}



void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	
//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);     //��ӳ��

   //���ø�����Ϊ�����������,���TIM2 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //TIM_CH1 PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 //���ø�����Ϊ�����������,���TIM2 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;        //TIM_CH2 PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ø�����Ϊ�����������,���TIM2 CH3��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM_CH3 PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ø�����Ϊ�����������,���TIM2 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;         //TIM_CH4 PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	//��ʼ��TIM3 PWMģʽ
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;      //���ô�װ�벶��ȽϼĴ���������ֵ

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM3   
}
