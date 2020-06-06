#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stm32f10x.h"
#include "Delay.h"
#include "sys.h"	 	 

#define KEY0 PAin(0)   //PB8
#define KEY1 PAin(1)   //PB9
#define KEY2 PAin(2)   //PA10
#define KEY3 PAin(3)   //PA11
#define KEY4 PAin(4)   //PA12
#define KEY5 PAin(5)   //PA13
#define KEY6 PAin(6)   //PA14
#define KEY7 PAin(7)   //PA15
	 
void Key_Init(void);//IO≥ı ºªØ
int8_t Key_Scan(void);	
int16_t Key_Input(void);

#endif
