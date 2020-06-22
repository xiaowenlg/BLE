#ifndef __sound_H_
#define __sound_H_
#include "system.h"

/* #define MOTO_Pin          GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
#define MOTO_Port        GPIOC
#define RCC_MOTO       RCC_APB2Periph_GPIOC */

/* 

/*  WTN6040时钟端口、引脚定义 */ // 语音芯片
#define WTN_PORT 			GPIOB   
#define WTN_PIN 			(GPIO_Pin_13|GPIO_Pin_12)
#define WTN_PORT_RCC		RCC_APB2Periph_GPIOB

#define  WTN_BUSY               PBin(15)   //忙检查I/O
#define  CLK_2A                 PBout(13) //时钟
#define  P_DATA_2A               PBout(12) //数据
#define SENSOR_1          PAin(0)   //干簧管
#define P_DATA PBout(12)
uint8_t  Line_2A_WTN6(uint8_t SB_DATA);
void SendNumber(uint8_t * N_Data,uint8_t len,uint8_t danwei);//本次您活动了.......
void SendNumber_hot(uint8_t * N_Data,uint8_t len);//消耗热量........
void Sendtim(uint8_t * F_Data,uint8_t lenf,uint8_t *M_Data,uint8_t lenm);//活动时间
void sound_Init(void);                 
void SensorIT_PB15_Init(void);//中断初始化
uint8_t CalData(uint16_t dat,uint8_t *res_dat);// 语音地址分解计算
extern u8 sound_flag;
extern uint16_t hotdistance;
extern uint8_t soundNum;//第几段语音
#endif

