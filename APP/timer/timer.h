


#ifndef __TIMER_H
#define __TIMER_H


#include "system.h"
#include "FreeRTOS.h"
#include "semphr.h"

void TIM3_Init(u16 per,u16 psc);
//extern uint16_t SM_Value;//�����ֵ
//extern uint8_t Key_OK;  //ȷ������ֵ
extern uint8_t TempCount;//�����ۼƱ���
extern uint8_t Freq;//Ƶ�ʱ���
extern uint16_t distance;
extern uint16_t data_timer;//�˶�ʱ��
extern uint16_t data_timer_play;//
extern uint16_t tmp_cal;
extern uint8_t TempCount_Heart;
extern uint8_t Freq_Heart;
extern u16 Power;
extern u8 Modbus_State;//Modbus״̬
extern int16_t iAllReg[16];//Modbus�Ĵ���
extern _Bool IsPlay; //������־λ
extern uint8_t serialdat_3[20];
extern uint8_t senddatflag;
extern uint8_t stopflag;//ϵͳ����ϵ��־λ
extern SemaphoreHandle_t  xSemaphore;
#endif


