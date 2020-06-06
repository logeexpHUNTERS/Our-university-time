#ifndef __STM32F10X_OLED_H_
#define __STM32F10X_OLED_H_

#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0
#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

/*！！！！！！！！！！！！！！！！！！哈重蕗苧！！！！！！！！！！！！！！！！！！！！*/
#define LED_PORT GPIOC      
#define LED_SCL GPIO_Pin_7     //D0
#define LED_SDA GPIO_Pin_6	  //D1
#define LED_RST GPIO_Pin_5
#define LED_DC  GPIO_Pin_4

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
/************************??????*******************************/
extern void LED_Init(void);
extern void LED_SetPos(unsigned char ucIdxX, unsigned char ucIdxY); 
extern void LED_P6x8Char(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucData);
extern void LED_P6x8Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[]);
extern void LED_P8x16Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[]);
extern void LED_P14x16Str(unsigned char ucIdxX,unsigned char ucIdxY,unsigned char ucDataStr[]);
extern void LED_PXx16MixStr(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char ucDataStr[]);
extern void LED_Fill(unsigned char ucData);
extern void LED_PrintChar(unsigned char ucIdxX, unsigned char ucIdxY, char cData);
extern void LED_PrintShort(unsigned char ucIdxX, unsigned char ucIdxY, short sData);
#endif
