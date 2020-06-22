#ifndef __sound_H_
#define __sound_H_
#include "system.h"

/* #define MOTO_Pin          GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
#define MOTO_Port        GPIOC
#define RCC_MOTO       RCC_APB2Periph_GPIOC */

/* 

/*  WTN6040ʱ�Ӷ˿ڡ����Ŷ��� */ // ����оƬ
#define WTN_PORT 			GPIOB   
#define WTN_PIN 			(GPIO_Pin_13|GPIO_Pin_12)
#define WTN_PORT_RCC		RCC_APB2Periph_GPIOB

#define  WTN_BUSY               PBin(15)   //æ���I/O
#define  CLK_2A                 PBout(13) //ʱ��
#define  P_DATA_2A               PBout(12) //����
#define SENSOR_1          PAin(0)   //�ɻɹ�
#define P_DATA PBout(12)
uint8_t  Line_2A_WTN6(uint8_t SB_DATA);
void SendNumber(uint8_t * N_Data,uint8_t len,uint8_t danwei);//���������.......
void SendNumber_hot(uint8_t * N_Data,uint8_t len);//��������........
void Sendtim(uint8_t * F_Data,uint8_t lenf,uint8_t *M_Data,uint8_t lenm);//�ʱ��
void sound_Init(void);                 
void SensorIT_PB15_Init(void);//�жϳ�ʼ��
uint8_t CalData(uint16_t dat,uint8_t *res_dat);// ������ַ�ֽ����
extern u8 sound_flag;
extern uint16_t hotdistance;
extern uint8_t soundNum;//�ڼ�������
#endif

