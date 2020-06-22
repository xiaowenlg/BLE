#ifndef __PWM_H_
#define __PWM_H_


#include "system.h"
//pwm的I/O口定义

#define PWM_PART            GPIOB
#define PWM_PIN             GPIO_Pin_6                                         //pb6脚
#define PWM_PART_RCC        RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO         //端口时钟和复用功能时钟
#define PWM_TIM             TIM4																							//pwm的定时器
#define PWM_TIM_RCC         RCC_APB1Periph_TIM4																//定时器时钟








void TIM4_CH1PWM_Init(u16 per,u16 psc);          //PWM初始化
#endif

