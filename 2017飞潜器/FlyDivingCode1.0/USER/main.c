#include "TIM_Int.h"
#include "delay.h"
//#include "key.h"
#include "sys.h"
//#include "usart.h"
#include "timer.h"

	
 int main(void)
 {		
 	
	DelayInit()	   ; 	     //延时函数初始化	

	//NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
	Pin_Init(); 	 					//串口初始化为9600
	TIM2_PWM_Init(9900,21);	   //不分频。PWM频率=72000/900=8Khz
	TIM4_PWM_Init(9999,106);
	 
  SkyOff;	WaterOn;// 空中

	TIM_SetCompare1(TIM4,9260);	//CH2
	TIM_SetCompare2(TIM4,9260);	//CH4
	TIM_SetCompare3(TIM4,9260);	//CH1
	TIM_SetCompare4(TIM4,9260); //CH3
	 
 	TIM_SetCompare1(TIM2,5500);	
	TIM_SetCompare2(TIM2,8420);	
	TIM_SetCompare3(TIM2,8000);	
	TIM_SetCompare4(TIM2,4850);
	delay_ms(2000);
	 
	TIM_SetCompare1(TIM2,8000);	
	TIM_SetCompare2(TIM2,6000);	
	TIM_SetCompare3(TIM2,5000);	
	TIM_SetCompare4(TIM2,8000);

  delay_ms(2000);
	 
	TIM_SetCompare1(TIM2,5500);	
	TIM_SetCompare2(TIM2,8420);	
	TIM_SetCompare3(TIM2,8000);	
	TIM_SetCompare4(TIM2,4850);
	 
	TIM_SetCompare1(TIM4,9260);	//CH2
	TIM_SetCompare2(TIM4,9260);	//CH4
	TIM_SetCompare3(TIM4,9260);	//CH1
	TIM_SetCompare4(TIM4,9260); //CH3
	 
	delay_ms(2000);
	
	TIM_SetCompare1(TIM4,9160);	//CH2
	TIM_SetCompare2(TIM4,9160);	//CH4
	TIM_SetCompare3(TIM4,9160);	//CH1
	TIM_SetCompare4(TIM4,9160); //CH3
	
	delay_ms(3000);
	
	TIM_SetCompare1(TIM2,8000);	
	TIM_SetCompare2(TIM2,6000);	
	TIM_SetCompare3(TIM2,5000);	
	TIM_SetCompare4(TIM2,8000);

	delay_ms(3000);
	
	
	TIM_SetCompare1(TIM2,5500);	
	TIM_SetCompare2(TIM2,8420);	
	TIM_SetCompare3(TIM2,8000);	
	TIM_SetCompare4(TIM2,4850);
	
	
	delay_ms(3000);
	
	TIM_SetCompare1(TIM4,9260);	//CH2
	TIM_SetCompare2(TIM4,9260);	//CH4
	TIM_SetCompare3(TIM4,9260);	//CH1
	TIM_SetCompare4(TIM4,9260); //CH3
	while(1);

  
 }

