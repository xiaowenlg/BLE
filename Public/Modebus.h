#ifndef __MODEBUS_H__
#define __MODEBUS_H__
#include "system.h"

uint16_t CRC16(uint8_t *puchMsg, uint16_t usDataLen);//CRCУ��
void ReadCoil(uint8_t *ReceBuff/*���ڽ��յ�������*/,_Bool *CoilM/*�ӻ���Ȧ*/);
void ReadRegisters(uint8_t *ReceBuff,int16_t *iAllReg);
void SetOneCoil(uint8_t *ReceBuff,_Bool *CoilM);//д������Ȧ ������:05
void WriteSinglReg(uint8_t *ReceBuff,int16_t *iAllRegM);
void ModeBusWork(uint8_t *ReceBuff,uint16_t *ia,_Bool *CoilX);
extern uint8_t SendBuff[256];
extern  _Bool Coil[32]; //�洢��Ȧ״̬������ȡ����I/O��
extern uint8_t DataLen;
#endif

