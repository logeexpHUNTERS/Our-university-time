#ifndef __TIM_Int_h
#define __TIM_Int_h

#include "sys.h"

#define WaterOn    PBout(13) = 1;
#define WaterOff   PBout(13) = 0;
#define SkyOn  PBout(12) = 1;
#define SkyOff PBout(12) = 0;
#define Up    PBin(15);
#define Down  PBin(14);

void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);
void Pin_Init(void);

#endif
