
#ifndef  __MYKEY_H__
#define  __MYKEY_H__
#include "system.h"

//按键在PE

//根据硬件连接定义键值
#define	KEY_VALUE_1              0x00fe    //PE0
#define	KEY_VALUE_2              0x00fd			//PE1
#define     KEY_VALUE_3              0x00fb				//PE2
#define     KEY_VALUE_4              0x00f7				//PE3
#define     KEY_VALUE_5		  0x00ef				//PE4
#define     KEY_VALUE_6		  0x00df
#define	KEY_NULL                    0x00ff			//PE5


//定义按键返回值状态(按下,长按,连_发,释放)
#define	KEY_DOWN               	0x8000
#define	KEY_LONG                         0x4000
#define	KEY_CONTINUE			0x2000
#define	KEY_UP     			0x1000   

void KeyInit(void);
static uint16_t KeyScan(void);
void  KeyLoop(uint16_t *pKeyValue);
void Key_All_Init(void);
#endif


