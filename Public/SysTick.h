#ifndef _SysTick_H
#define _SysTick_H

#include "system.h"
#include "FreeRTOS.h"
#include "task.h"
void SysTick_Init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
extern TaskHandle_t task1_Handler;
extern TaskHandle_t task2_Handler;
extern TaskHandle_t task3_Handler;


#endif
