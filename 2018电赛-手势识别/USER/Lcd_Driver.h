#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "stm32f10x.h"

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0  00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1  00000 000000 00000
#define GRAY2   0x4208      			//��ɫ2  1111111111011111
#define LightYellow  0xccffcc   //ǳ��ɫ
#define LightPurple  0xccccff  //ǳ��ɫ
#define DarkPurple   0x9999  //����ɫ
#define DarkRad      0xff9900 //����ɫ
#define SkyBlue      0x777777  //����ɫ





#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160       //���ص��С

/*�����������������������������������й�TFT LCD���ŵĶ��塪��������������������������������������������������*/
//#define LCD_GPIO   GPIOA		//����TFT���ݶ˿�

#define LCD_SCL   	GPIO_Pin_12	//C13	
#define LCD_SDA    	GPIO_Pin_11  // C0		
#define LCD_CS     	GPIO_Pin_8  	//A4

//#define LCD_LED    	GPIO_Pin_6  	//TFT-BL  A6
#define LCD_RS     	GPIO_Pin_9		//TFT-DC A0
#define LCD_RST   	GPIO_Pin_10		//TFT-RES  C2

#define	LCD_SCL_SET  	GPIOA->BSRR=LCD_SCL    
#define	LCD_SDA_SET  	GPIOA->BSRR=LCD_SDA   
#define	LCD_CS_SET  	GPIOA->BSRR=LCD_CS     
//#define	LCD_LED_SET  	GPIOA->BSRR=LCD_LED  
#define	LCD_RS_SET  	GPIOA->BSRR=LCD_RS 
#define	LCD_RST_SET  	GPIOA->BSRR=LCD_RST   //Һ�����ƿ���λ�������ĺ궨��

#define	LCD_SCL_CLR  	GPIOA->BRR=LCD_SCL  
#define	LCD_SDA_CLR  	GPIOA->BRR=LCD_SDA 
#define	LCD_CS_CLR  	GPIOA->BRR=LCD_CS 		 
//#define	LCD_LED_CLR  	GPIOA->BRR=LCD_LED
#define	LCD_RST_CLR  	GPIOA->BRR=LCD_RST
#define	LCD_RS_CLR  	GPIOA->BRR=LCD_RS			//Һ�����ƿڸ�λ�������ĺ궨��   

void Lcd_Init(void);
void Lcd_Clear(unsigned short Color);
void LCD_P6x8Char(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, unsigned char s);
void LCD_P6x8Str(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, unsigned char *s);
void LCD_P6x8Value(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc,int data);
void LCD_P8x16Char(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, unsigned char s);
void LCD_P8x16Str(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, unsigned char *s);
void LCD_P8x16Value(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc,int data);
#define IMAGE 1         //��������
void LCD_ClearImage(unsigned short x, unsigned short y);
void LCD_PutNum(unsigned short x, unsigned short y,unsigned char num);
void LCD_PutRPS(unsigned short x, unsigned short y,unsigned char num) ;//0~2 ȭ����������

#endif
