#ifndef _SENSORIO_H
#define _SENSORIO_H

#include "system.h"

/*  �ɻɹ�ʱ�Ӷ˿ڡ����Ŷ��� */
#define SENSOR_PORT 			GPIOA                                  //PA12
#define SENSOR_PIN 			GPIO_Pin_12
#define SENSOR_PORT_RCC		RCC_APB2Periph_GPIOA
 


//���ʴ�����
#define SENSOR_B_PORT 			GPIOA   
#define SENSOR_B_PIN 			GPIO_Pin_15
#define SENSOR_B_PORT_RCC		RCC_APB2Periph_GPIOA

//�̵���
#define JDQ_PORT                             GPIOB
#define JDQ_PIN                               GPIO_Pin_8
#define JDQ_RCC                              RCC_APB2Periph_GPIOB

#define SENSOR_2          PAin(15)  //���ʴ�����
#define JDQ                   PBout(8) //�̵�������
void SENSOR_Init(void);
void SENSOR2_Init(void);
void JDQ_Init(void);
void SensorReData(void);
void SensorReHeart(void);
void SensorIT_PA0_Init();

#endif
