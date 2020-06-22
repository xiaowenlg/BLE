#ifndef __MOTO_H_
#define __MOTO_H_
#include "system.h"

#define MOTO_Pin          GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
#define MOTO_Port        GPIOC
#define RCC_MOTO       RCC_APB2Periph_GPIOC



void MotoInit(void);
void MotoCw(u16 n);
#endif

