#ifndef __SMG4_H_
#define __SMG4_H_
#include "system.h"
#define SMG_PER  (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)    //����ܶ�
#define SMG_PER_RCC		RCC_APB2Periph_GPIOD																																			//�˿�ʱ��
#define  SMG_PER_IO        GPIOD

#define SMG_WEI   (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3)//�����λ
#define SMG_WEI_RCC		RCC_APB2Periph_GPIOF																																						//�˿�ʱ��
#define SMG_WEI_IO       GPIOF



void SMG_IO_Init(void);
//void Disply(uint16_t x);
//void Disply_A(int x);
#endif
