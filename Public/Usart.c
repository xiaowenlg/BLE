/*
*串口1通信  PA9:RXD                  PA10:TXD

*/

#include "Usart.h"
#include "led.h"
#include "FreeRTOS.h"					//FreeRTOSê1ó?
#include "task.h"
#include "queue.h"	 	 //uint8_t Buffer[300];//接收数据缓冲区
 u8 DataIndex = 0;
 uint8_t DataLen;
 uint8_t Usart2DataLen = 0;
 uint8_t Usart2DataIndex = 0;

 uint8_t Usart3DataLen = 0;
 uint8_t Usart3DataIndex = 0;
u8 Modbus_State = 1;
int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART1->SR;
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

void UartSendData(uint8_t *s)
{
	USART1->SR;
	while (*s)
	{
		USART_SendData(USART1,*s++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}

void UartASendStr (u8 *pucStr, u8 ulNum) 
{ 
	u8 i;
	
	USART1->SR;
	//发送数据
	for(i = 0;i<ulNum;i++) 
	{ 
		// USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_SendData(USART1,*pucStr++); 
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		
		
	} 

} 


void SendByte(u8 byt) 
{
	USART_SendData(USART1,byt);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	
}
void SendByte2(u8 byt) 
{
	USART_SendData(USART2,byt);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
	
}
void SendByte3(u8 byt) 
{
	USART_SendData(USART3,byt);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET);
	
}
void Uart1SendData(uint8_t *s)
{
	//USART1->SR;
	while (*s)
	{
		USART_SendData(USART1,*s++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}
void Uart2SendData(uint8_t *s)
{
	//USART1->SR;
	while (*s)
	{
		USART_SendData(USART2,*s++);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}
void Uart3SendData (u8 *pucStr, u8 ulNum) 
{ 
	u8 i; 
	USART3->SR;
	for(i = 0;i<ulNum;i++) 
	{ 
		 USART_ClearFlag(USART3,USART_FLAG_TC);
		USART_SendData(USART3,*pucStr++); 
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		
		
	} 

} 
/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 


void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //GPIO结构体
	NVIC_InitTypeDef NVIC_InitStructure;     //中断优先级结构体
	USART_InitTypeDef USART_InitStructure;		//串口结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);   ////打开时钟
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;					//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  			/* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;				//RX			      //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); 				/* 初始化GPIO */
	
	//Usart初始化
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_Init(USART1,&USART_InitStructure);//初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_RXNE);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启传送完一帧中断---------------------****************************
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	
	u8 tempData = 0,i;
	 u8 clear=clear;  
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		
		ReceData[DataIndex] = 0;
		tempData = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		ReceData[DataIndex] = tempData;
		DataIndex++;
		//USART_SendData(USART1,DataIndex);
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	} 
	else if (USART_GetITStatus(USART1,USART_IT_IDLE) != Bit_RESET )
	{
		clear=USART1->SR;
		clear=USART1->DR;
		DataLen = DataIndex;
		DataIndex = 0;
		//UartASendStr(ReceData,8);
		ReceOver = 1;
		//xQueueSendFromISR(Message_Queue,ReceData,&xHigherPriorityTaskWoken);//?ò?óáD?D·￠?íêy?Y
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//è?1?Dèòaμ??°??DDò?′?è????D??
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_NE) != RESET)
	{
		USART_ClearFlag(USART1,USART_FLAG_NE);
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_FE) != RESET)
	{
		USART_ClearFlag(USART1,USART_FLAG_FE);
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_PE) != RESET)
	{
		USART_ClearFlag(USART1,USART_FLAG_PE);


		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_ClearFlag(USART1,USART_FLAG_TC);
	}
} 

/*******************************************************************************
* 函 数 名         : USART2_Init
* 函数功能		   : USART2初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 


void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //GPIO结构体
	NVIC_InitTypeDef NVIC_InitStructure;     //中断优先级结构体
	USART_InitTypeDef USART_InitStructure;		//串口结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);   ////打开时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;					//TX			   //串口输出PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  			/* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;				//RX			      //串口输入PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); 				/* 初始化GPIO */
	
	//Usart初始化
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_Init(USART2,&USART_InitStructure);//初始化串口2
	
	USART_Cmd(USART2, ENABLE);  //使能串口2
	
	USART_ClearFlag(USART2, USART_FLAG_TC|USART_FLAG_RXNE);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启传送完一帧中断---------------------****************************
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}	

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	
	u8 tempData = 0;
	 u8 clear=clear;  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Usart2Data[Usart2DataIndex] = 0;
		tempData = USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		Usart2Data[Usart2DataIndex] = tempData;
		Usart2DataIndex++;
		//USART_SendData(USART1,DataIndex);
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	} 
	else if (USART_GetITStatus(USART2,USART_IT_IDLE) != Bit_RESET )
	{
		clear=USART2->SR;
		clear=USART2->DR;
		Usart2DataLen = Usart2DataIndex;
		Usart2DataIndex = 0;
		//UartASendStr(ReceData,8);
		Usart2ReceOver = 1;
	}
	USART_ClearFlag(USART2,USART_FLAG_TC);
} 
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //GPIO结构体
	NVIC_InitTypeDef NVIC_InitStructure;     //中断优先级结构体
	USART_InitTypeDef USART_InitStructure;		//串口结构体
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;					//TX			   //串口输出PB10
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);  			/* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;				//RX			      //串口输入PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOB,&GPIO_InitStructure); 				/* 初始化GPIO */
	
	//Usart初始化
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_Init(USART3,&USART_InitStructure);//初始化串口3
	
	USART_Cmd(USART3, ENABLE);  //使能串口2
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启传送完一帧中断---------------------****************************
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

void USART3_IRQHandler(void)                	//串口2中断服务程序
{
	
	u8 tempData = 0;
	 u8 clear=clear;  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		Usart3Data[Usart3DataIndex] = 0;
		tempData = USART_ReceiveData(USART3);//(USART2->DR);	//读取接收到的数据
		Usart3Data[Usart3DataIndex] = tempData;
		Usart3DataIndex++;
		//USART_SendData(USART1,DataIndex);
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	} 
	else if (USART_GetITStatus(USART3,USART_IT_IDLE) != Bit_RESET )
	{
		clear=USART3->SR;
		clear=USART3->DR;
		Usart3DataLen = Usart3DataIndex;
		Usart3DataIndex = 0;
		//UartASendStr(ReceData,8);
		Usart3ReceOver = 1;
	}
	USART_ClearFlag(USART3,USART_FLAG_TC);
} 
