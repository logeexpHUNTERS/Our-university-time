#include "stm32f10x.h"
#include "Delay.h"
#include "stm32f10x_tim.h"
#include "string.h"
#include "oled.h"
#include "nrf24l01.h"
#include "Syn.h"
#include "timer.h"
void Init(void);
uint16_t absdiff(uint16_t a, uint16_t b);
uint16_t groupMovAvg(uint16_t data, uint16_t* group);

u8 TXData[1] = {'A'};
u8 RXData[1] = {'0'};

#define Equipment 1     // 宏定义 1表示主设备，0表示从设备

#if  Equipment
/************************
超声波接收器
硬件SPI_1
***********************/
extern u8 exti2Flag;
extern u8 exti9_5Flag;
extern u8 exti15_10Flag;

int ti2Tim  = 0;
int ti9Tim  = 0;
int ti12Tim = 0;

int time = 0;

extern uint32_t usCnt;
extern uint32_t msCnt ;
extern uint32_t us5_Cnt;

int main()
{  
  uint16_t Xw1, Xw2, Xw3;
  uint16_t X1,  X2,  X3;
  uint16_t Dt1[10] = {0};
  uint16_t Dt2[10] = {0};
  uint16_t Dt3[10] = {0};
  uint16_t dispX1 = 0;
  uint16_t dispX2 = 0;
  uint16_t dispX3 = 0;
  uint16_t tempX1 = 0;
  uint16_t tempX2 = 0;
  uint16_t tempX3 = 0;
  
  u8 StartFlag = 0;
  u8 dataOutFlag = 0;
  
  Init();
  TIM3_Init(359,0); // 5us
	while(NRF24L01_Check()); 	
  OLED_P8x16Str(10,0,"Space Locator"); 
  
  OLED_P8x16Str(0,2,"W1:");
  OLED_P8x16Str(0,4,"W2:");
  OLED_P8x16Str(0,6,"W3:");
  
  NRF24L01_RX_Mode();
  
	while(1)
	{
    while(NRF24L01_RxPacket(RXData) && RXData[0] == 'A')
    { 
      RXData[0] = '0';
         
      TIMxOFF_ON(TIM3,ON);
      EXTI_TIMxOFF_ON(TIM3,ON);
      EXTILineOFF_ON(EXTI_Line2, EXTI_ON);
      EXTILineOFF_ON(EXTI_Line9, EXTI_ON);
      EXTILineOFF_ON(EXTI_Line12,EXTI_ON);
      
      while(exti2Flag != '1' || exti9_5Flag != '1' || exti15_10Flag != '1');
      exti2Flag     = 0;
      exti9_5Flag   = 0;
      exti15_10Flag = 0;
      
      TIMxOFF_ON(TIM3,OFF); 
      EXTI_TIMxOFF_ON(TIM3,OFF);
      EXTILineOFF_ON(EXTI_Line2, EXTI_OFF);
      EXTILineOFF_ON(EXTI_Line9, EXTI_OFF);
      EXTILineOFF_ON(EXTI_Line12,EXTI_OFF);
       
      Xw1 = ti2Tim*17/100; // x * 5us * 0.034cm/s
      Xw2 = ti9Tim*17/100;
      Xw3 = ti12Tim*17/100;
      
      if(StartFlag < 10)
      {
        X1 = Xw1;
        X2 = Xw2;
        X3 = Xw3;
        if(absdiff(Xw1,X1)<50)
          X1 = Xw1;
        if(absdiff(Xw2,X2)<50)
          X2 = Xw2;
        if(absdiff(Xw1,X1)<50)
          X3 = Xw3;
        tempX1 = groupMovAvg(X1, Dt1);
        tempX2 = groupMovAvg(X2, Dt2);
        tempX3 = groupMovAvg(X3, Dt3);
        StartFlag ++;
      }
      else
      {
        if(absdiff(Xw1,X1)<50)
          tempX1 = Xw1;
        if(absdiff(Xw2,X2)<50)
          tempX2 = Xw2;
        if(absdiff(Xw1,X1)<50)
          tempX3 = Xw3;
        X1 = groupMovAvg(tempX1, Dt1);
        X2 = groupMovAvg(tempX2, Dt2);
        X3 = groupMovAvg(tempX3, Dt3);
        if(dataOutFlag == 0)
        {
          dispX1 = X1;
          dispX2 = X2;
          dispX3 = X3;
          dataOutFlag = 1;
        }
        else
        {
          if(absdiff(X1,dispX1)<30)
            dispX1 = X1;
          if(absdiff(X2,dispX2)<30)
            dispX2 = X2;
          if(absdiff(dispX3,X3)<30)
            dispX3 = X3;
        }
      }
      
      OLED_P8x16Value(20,2,dispX1);
      OLED_P8x16Value(20,4,dispX2);
      OLED_P8x16Value(20,6,dispX3);
      
      ti2Tim  = 0;
      ti9Tim  = 0;
      ti12Tim = 0;
      us5_Cnt = 0;
      
    } 
	}
}



#elif !Equipment
/************************************************
超声波发射接口：  PC8,40k Hz,Timer 3 Ch3  REMAP
NRF24L01接口：    硬件SPI接口
*************************************************/
int main()
{ 
  Init();
  
  TIM3_Init(19,89); //40kHz
  
	while(NRF24L01_Check()); 
  TIMxOFF_ON(TIM3,OFF);
  
  NRF24L01_TX_Mode();
  
	while(1)
	{   
    while(!NRF24L01_TxPacket(TXData));
    TIMxOFF_ON(TIM3,ON);
    delayus(250);
    TIMxOFF_ON(TIM3,OFF);
    delayms(49);
	}
}

#endif


void Init()
{
  #if Equipment
  
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    DelayInit();         
    OLED_Init();
    //TIM4_Init(719,0); //10us
  
    recPinInit();
    NRF24L01_Init();
    
    TIMxOFF_ON(TIM3,OFF); 
    EXTI_TIMxOFF_ON(TIM3,OFF);
    EXTILineOFF_ON(EXTI_Line2, EXTI_OFF);
    EXTILineOFF_ON(EXTI_Line9, EXTI_OFF);
    EXTILineOFF_ON(EXTI_Line12,EXTI_OFF);
  
  
  #elif !Equipment
  
    SystemInit();
    DelayInit(); 
    SendPort_Init();
    NRF24L01_Init();
    JTAG_Set(1); 
    
  #endif
}


uint16_t absdiff(uint16_t a, uint16_t b)
{
  int diff;
  
  diff = a - b;
  
  if(diff > 0)
    return (uint16_t)diff;
  else if(diff < 0)
    return (uint16_t)(-diff);
  else if(diff == 0)
    return (uint16_t)0;
  else
    return 0;
}

uint16_t groupMovAvg(uint16_t data, uint16_t* group)
{
	u8 i;
	uint16_t avgData;
	for(i=0; i<10; i++)
	{
		group[i+9] = group[i+8];
		group[i+8] = group[i+7];
		group[i+7] = group[i+6];
		group[i+6] = group[i+5];
		group[i+5] = group[i+4];
		group[i+4] = group[i+3];
		group[i+3] = group[i+2];
		group[i+2] = group[i+1];
		group[i+1] = group[i];
		group[i]   = data;	
	}
	
	avgData = (group[0]+group[1]+group[2]+group[3]+group[4]
			      +group[5]+group[6]+group[7]+group[8]+group[9])/10;
	
	return avgData;
}

