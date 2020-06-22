


#ifndef __TIMER_H
#define __TIMER_H


#include "system.h"
#include "FreeRTOS.h"
#include "semphr.h"

void TIM3_Init(u16 per,u16 psc);
//extern uint16_t SM_Value;//数码管值
//extern uint8_t Key_OK;  //确定按键值
extern uint8_t TempCount;//次数累计变量
extern uint8_t Freq;//频率变量
extern uint16_t distance;
extern uint16_t data_timer;//运动时间
extern uint16_t data_timer_play;//
extern uint16_t tmp_cal;
extern uint8_t TempCount_Heart;
extern uint8_t Freq_Heart;
extern u16 Power;
extern u8 Modbus_State;//Modbus状态
extern int16_t iAllReg[16];//Modbus寄存器
extern _Bool IsPlay; //播报标志位
extern uint8_t serialdat_3[20];
extern uint8_t senddatflag;
extern uint8_t stopflag;//系统请求断电标志位
extern SemaphoreHandle_t  xSemaphore;
#endif


