#ifndef __MODEBUS_H__
#define __MODEBUS_H__
#include "system.h"

uint16_t CRC16(uint8_t *puchMsg, uint16_t usDataLen);//CRC校验
void ReadCoil(uint8_t *ReceBuff/*串口接收到的数据*/,_Bool *CoilM/*从机线圈*/);
void ReadRegisters(uint8_t *ReceBuff,int16_t *iAllReg);
void SetOneCoil(uint8_t *ReceBuff,_Bool *CoilM);//写单个线圈 功能码:05
void WriteSinglReg(uint8_t *ReceBuff,int16_t *iAllRegM);
void ModeBusWork(uint8_t *ReceBuff,uint16_t *ia,_Bool *CoilX);
extern uint8_t SendBuff[256];
extern  _Bool Coil[32]; //存储线圈状态，这里取两个I/O口
extern uint8_t DataLen;
#endif

