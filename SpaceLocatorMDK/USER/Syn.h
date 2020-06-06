#ifndef _Syn_h
#define _Syn_h

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "Delay.h"
#include "string.h"
#include "sys.h"
#include "timer.h"


typedef enum {EXTI_OFF = 0, EXTI_ON = 1} EXTIxLine;

void JTAG_Set(u8 mode);
void SendPort_Init(void);
void recPinInit(void);
void EXTI2_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void EXTILineOFF_ON(uint32_t EXTI_Line, EXTIxLine State);

#endif
