#ifndef __PWM_H_
#define __PWM_H_


#include "system.h"
//pwm��I/O�ڶ���

#define PWM_PART            GPIOB
#define PWM_PIN             GPIO_Pin_6                                         //pb6��
#define PWM_PART_RCC        RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO         //�˿�ʱ�Ӻ͸��ù���ʱ��
#define PWM_TIM             TIM4																							//pwm�Ķ�ʱ��
#define PWM_TIM_RCC         RCC_APB1Periph_TIM4																//��ʱ��ʱ��








void TIM4_CH1PWM_Init(u16 per,u16 psc);          //PWM��ʼ��
#endif

