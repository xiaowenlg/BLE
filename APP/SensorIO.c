
/*
	传感器初始化

*/

#include "SensorIO.h"

/*******************************************************************************
* 函 数 名         : SENSOR_Init
* 函数功能	       : 干簧管引脚初始化 PA6
* 输    入          : 无
* 输    出          : 无
*******************************************************************************/


//心率传感器变量
uint8_t TempCount_Heart = 0;
uint8_t nowheart = 1,lastheart = 0;

void SENSOR_Init() //PA12   //干簧管接口 用于中断
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(SENSOR_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //设置成上拉输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	//GPIO_SetBits(LED_PORT,LED_PIN);   //将LED端口拉高，熄灭所有LED
}

void SENSOR2_Init(void)   //PA15        //心率传感器
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(SENSOR_B_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //设置成下拉输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(SENSOR_B_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
}

void JDQ_Init(void)  //继电器初始化  //PB8
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(JDQ_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=JDQ_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 // //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(JDQ_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
}


void SensorReHeart(void)
{
	nowheart = SENSOR_2 ;
	if (nowheart != lastheart)
	{
		if(nowheart==1)
			TempCount_Heart++;
	}
	lastheart = nowheart;
}
void SensorIT_PA12_Init()// 磁控开关
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	NVIC_InitTypeDef NVIC_InitTypeDef; //NVIC 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SENSOR_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);//开外设端口时钟和复用时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //设置成上拉输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(SENSOR_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);//映射中断线
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式配置 EXTI_Trigger_RisingEXTI_Trigger_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//3?ê??ˉ?D??
	
	//中断优先级
	NVIC_InitTypeDef.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitTypeDef.NVIC_IRQChannelPreemptionPriority = 0x02;                          //?à??ó??è?? 2￡?
	NVIC_InitTypeDef.NVIC_IRQChannelSubPriority = 0x01;																						//×óó??è?? 2
	NVIC_InitTypeDef.NVIC_IRQChannelCmd = ENABLE;																							//ê1?üía2??D??í¨μà
	NVIC_Init(&NVIC_InitTypeDef);						
	
}
void SensorIT_PA0_Init()// 磁控开关
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	NVIC_InitTypeDef NVIC_InitTypeDef; //NVIC 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SENSOR_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);//开外设端口时钟和复用时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //设置成上拉输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(SENSOR_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//映射中断线
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式配置 EXTI_Trigger_RisingEXTI_Trigger_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//3?ê??ˉ?D??
	
	//中断优先级
	NVIC_InitTypeDef.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitTypeDef.NVIC_IRQChannelPreemptionPriority = 0x02;                          //?à??ó??è?? 2￡?
	NVIC_InitTypeDef.NVIC_IRQChannelSubPriority = 0x01;																						//×óó??è?? 2
	NVIC_InitTypeDef.NVIC_IRQChannelCmd = ENABLE;																							//ê1?üía2??D??í¨μà
	NVIC_Init(&NVIC_InitTypeDef);						
	
}