#ifndef __USART_H__
#define __USART_H__

#include "system.h"
#include "stdio.h"
int fputc(int ch,FILE *p);
void USART1_Init(u32 bound);
void USART2_Init(u32 bound);  
void USART3_Init(u32 bound);  
void UartSendData(uint8_t *s);// ����1��������
void Uart3SendData (u8 *pucStr, u8 ulNum);
void Uart2SendData(uint8_t *s);// ����2��������
void Uart1SendData(uint8_t *s);// ����3��������
void UartASendStr (u8 *pucStr, u8 ulNum);
void SendByte(u8 byt) ;// ����1���͵��ֽ�
void SendByte2(u8 byt);// ����2���͵��ֽ�
void SendByte3(u8 byt);// ����3���͵��ֽ�


extern uint8_t ReceData[50];//�������ݻ�����
extern uint8_t Usart2Data[256];//Usart2���ݻ�����
extern uint8_t Usart3Data[256];//Usart3���ݻ�����

extern _Bool ReceOver;//������ϱ�־
extern _Bool Usart2ReceOver;//������ϱ�־
extern _Bool Usart3ReceOver;//������ϱ�־

#endif
