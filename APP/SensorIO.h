#ifndef _SENSORIO_H
#define _SENSORIO_H

#include "system.h"

/*  干簧管时钟端口、引脚定义 */
#define SENSOR_PORT 			GPIOA                                  //PA12
#define SENSOR_PIN 			GPIO_Pin_12
#define SENSOR_PORT_RCC		RCC_APB2Periph_GPIOA
 


//心率传感器
#define SENSOR_B_PORT 			GPIOA   
#define SENSOR_B_PIN 			GPIO_Pin_15
#define SENSOR_B_PORT_RCC		RCC_APB2Periph_GPIOA

//继电器
#define JDQ_PORT                             GPIOB
#define JDQ_PIN                               GPIO_Pin_8
#define JDQ_RCC                              RCC_APB2Periph_GPIOB

#define SENSOR_2          PAin(15)  //心率传感器
#define JDQ                   PBout(8) //继电器引脚
void SENSOR_Init(void);
void SENSOR2_Init(void);
void JDQ_Init(void);
void SensorReData(void);
void SensorReHeart(void);
void SensorIT_PA0_Init();

#endif
