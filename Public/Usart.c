/*
*����1ͨ��  PA9:RXD                  PA10:TXD

*/

#include "Usart.h"
#include "led.h"
#include "FreeRTOS.h"					//FreeRTOS��1��?
#include "task.h"
#include "queue.h"	 	 //uint8_t Buffer[300];//�������ݻ�����
 u8 DataIndex = 0;
 uint8_t DataLen;
 uint8_t Usart2DataLen = 0;
 uint8_t Usart2DataIndex = 0;

 uint8_t Usart3DataLen = 0;
 uint8_t Usart3DataIndex = 0;
u8 Modbus_State = 1;
int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
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
	//��������
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
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 


void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //GPIO�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;     //�ж����ȼ��ṹ��
	USART_InitTypeDef USART_InitStructure;		//���ڽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);   ////��ʱ��
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;					//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  			/* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;				//RX			      //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 				/* ��ʼ��GPIO */
	
	//Usart��ʼ��
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART1,&USART_InitStructure);//��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_RXNE);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//����������һ֡�ж�---------------------****************************
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	
	u8 tempData = 0,i;
	 u8 clear=clear;  
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		
		ReceData[DataIndex] = 0;
		tempData = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
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
		//xQueueSendFromISR(Message_Queue,ReceData,&xHigherPriorityTaskWoken);//?��?����D?D����?����y?Y
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//��?1?D����a��??��??DD��?��?��????D??
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
* �� �� ��         : USART2_Init
* ��������		   : USART2��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 


void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //GPIO�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;     //�ж����ȼ��ṹ��
	USART_InitTypeDef USART_InitStructure;		//���ڽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);   ////��ʱ��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;					//TX			   //�������PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  			/* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;				//RX			      //��������PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 				/* ��ʼ��GPIO */
	
	//Usart��ʼ��
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART2,&USART_InitStructure);//��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
	
	USART_ClearFlag(USART2, USART_FLAG_TC|USART_FLAG_RXNE);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//����������һ֡�ж�---------------------****************************
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}	

void USART2_IRQHandler(void)                	//����2�жϷ������
{
	
	u8 tempData = 0;
	 u8 clear=clear;  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Usart2Data[Usart2DataIndex] = 0;
		tempData = USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
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
	GPIO_InitTypeDef GPIO_InitStructure;     //GPIO�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;     //�ж����ȼ��ṹ��
	USART_InitTypeDef USART_InitStructure;		//���ڽṹ��
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;					//TX			   //�������PB10
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);  			/* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;				//RX			      //��������PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOB,&GPIO_InitStructure); 				/* ��ʼ��GPIO */
	
	//Usart��ʼ��
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART3,&USART_InitStructure);//��ʼ������3
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���2
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//����������һ֡�ж�---------------------****************************
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void USART3_IRQHandler(void)                	//����2�жϷ������
{
	
	u8 tempData = 0;
	 u8 clear=clear;  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Usart3Data[Usart3DataIndex] = 0;
		tempData = USART_ReceiveData(USART3);//(USART2->DR);	//��ȡ���յ�������
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
