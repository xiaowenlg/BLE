#include "smg4.h"
#include "timer.h"



void SMG_IO_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
		RCC_APB2PeriphClockCmd(SMG_PER_RCC,ENABLE);
	  RCC_APB2PeriphClockCmd(SMG_WEI_RCC,ENABLE);
		
	
		//段配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Pin = SMG_PER;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(SMG_PER_IO,&GPIO_InitStructure);
	//位配置

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Pin = SMG_WEI;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOF,&GPIO_InitStructure);
	
		GPIO_Write(SMG_PER_IO,0xffff);
		GPIO_Write(GPIOF,0xffff);
}




