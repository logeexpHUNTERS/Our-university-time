#ifndef _TFT_LCD_H
#define _TFT_LCD_H 

#include "stm32f10x.h"

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111
#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

/*—————————————————有关TFT LCD引脚的定义——————————————————————————*/
#define LCD_CTRLB   GPIOB		//定义TFT数据端口

#define LCD_SCL   	GPIO_Pin_13		//PB13--->>TFT --SCL
#define LCD_SDA    	GPIO_Pin_15		//PB15 MOSI--->>TFT --SDA
#define LCD_CS     	GPIO_Pin_12  	//MCU_PB12--->>TFT --CS

#define LCD_LED    	GPIO_Pin_1  	//MCU_PB9--->>TFT --BL
#define LCD_RS     	GPIO_Pin_10		//PB10--->>TFT --DC
#define LCD_RST   	GPIO_Pin_11		//PB10--->>TFT --RES

#define	LCD_SCL_SET  	LCD_CTRLB->BSRR=LCD_SCL    
#define	LCD_SDA_SET  	LCD_CTRLB->BSRR=LCD_SDA   
#define	LCD_CS_SET  	LCD_CTRLB->BSRR=LCD_CS     
#define	LCD_LED_SET  	LCD_CTRLB->BSRR=LCD_LED   
#define	LCD_RS_SET  	LCD_CTRLB->BSRR=LCD_RS 
#define	LCD_RST_SET  	LCD_CTRLB->BSRR=LCD_RST   //液晶控制口置1操作语句宏定义

#define	LCD_SCL_CLR  	LCD_CTRLB->BRR=LCD_SCL  
#define	LCD_SDA_CLR  	LCD_CTRLB->BRR=LCD_SDA 
#define	LCD_CS_CLR  	LCD_CTRLB->BRR=LCD_CS 		 
#define	LCD_LED_CLR  	LCD_CTRLB->BRR=LCD_LED 
#define	LCD_RST_CLR  	LCD_CTRLB->BRR=LCD_RST
#define	LCD_RS_CLR  	LCD_CTRLB->BRR=LCD_RS			//液晶控制口置0操作语句宏定义   

#define USE_ONCHIP_FLASH_FONT 1          //定义是否在片内存储字体、图片的模


void Lcd_Init(void);
void showimage(const unsigned char *p);
void Lcd_Clear(u16 Color);
void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
#endif
