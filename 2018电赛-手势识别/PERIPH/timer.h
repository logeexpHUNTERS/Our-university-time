#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"

void TIM1_Config(uint16_t Period,uint16_t Prescaler);
void TIM2_Config(uint16_t Period,uint16_t Prescaler);
//void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);
void TIM3_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);

#endif
