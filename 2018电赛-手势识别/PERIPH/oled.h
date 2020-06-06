#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	

//-----------------OLED端口定义----------------  	    				   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOG,GPIO_Pin_7)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOG,GPIO_Pin_7)
#define OLED_RST_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_5)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOG,GPIO_Pin_5)
#define OLED_DC_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_6)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOG,GPIO_Pin_6)
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_3)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOG,GPIO_Pin_3)
#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_4)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOG,GPIO_Pin_4)
	     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_WR_Byte(u8 dat,u8 cmd);	    	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_P6x8Char(u8 x,u8 y,u8 chr);
void OLED_P8x16Char(u8 x,u8 y,u8 chr);
void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
void OLED_P6x8Value(unsigned short x, unsigned short y, int data);
void OLED_P8x16Value(unsigned short x, unsigned short y,int data);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 

