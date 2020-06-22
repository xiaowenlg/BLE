#ifndef __USART_H__
#define __USART_H__

#include "system.h"
#include "stdio.h"
int fputc(int ch,FILE *p);
void USART1_Init(u32 bound);
void USART2_Init(u32 bound);  
void USART3_Init(u32 bound);  
void UartSendData(uint8_t *s);// 串口1发送数组
void Uart3SendData (u8 *pucStr, u8 ulNum);
void Uart2SendData(uint8_t *s);// 串口2发送数组
void Uart1SendData(uint8_t *s);// 串口3发送数组
void UartASendStr (u8 *pucStr, u8 ulNum);
void SendByte(u8 byt) ;// 串口1发送单字节
void SendByte2(u8 byt);// 串口2发送单字节
void SendByte3(u8 byt);// 串口3发送单字节


extern uint8_t ReceData[50];//接收数据缓冲区
extern uint8_t Usart2Data[256];//Usart2数据缓冲区
extern uint8_t Usart3Data[256];//Usart3数据缓冲区

extern _Bool ReceOver;//接收完毕标志
extern _Bool Usart2ReceOver;//接收完毕标志
extern _Bool Usart3ReceOver;//接收完毕标志

#endif
