#include "delay.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"

unsigned int TimeDelay;	 //声明的全局变量
void DelayInit(void)
{
	if(SysTick_Config(SystemCoreClock / 1000000)) //配置为1us一次中断
	{
		while(1);  	//如果配置失败则死循环
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //失能滴答定时器，需要时再使能
}

void delay_us(unsigned int us)
{
	TimeDelay = us;	 						   //us赋值给全局变量TD
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //使能滴答定时器，允许中断触发
	while(TimeDelay != 0);					   //在中断中自减
}	

void delay_ms(unsigned int ms)
{
	delay_us(ms * 1000);		//1ms = 1000us
}

void SysTick_Handler(void)						//在中断中自减
{
	if (TimeDelay != 0)
	{ 
		TimeDelay--;
	}	
}

void Software_Delay(unsigned int sec)
{
	for(;sec>0;sec--);
}
