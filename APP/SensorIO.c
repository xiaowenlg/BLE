
/*
	��������ʼ��

*/

#include "SensorIO.h"

/*******************************************************************************
* �� �� ��         : SENSOR_Init
* ��������	       : �ɻɹ����ų�ʼ�� PA6
* ��    ��          : ��
* ��    ��          : ��
*******************************************************************************/


//���ʴ���������
uint8_t TempCount_Heart = 0;
uint8_t nowheart = 1,lastheart = 0;

void SENSOR_Init() //PA12   //�ɻɹܽӿ� �����ж�
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(SENSOR_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //���ó���������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	//GPIO_SetBits(LED_PORT,LED_PIN);   //��LED�˿����ߣ�Ϩ������LED
}

void SENSOR2_Init(void)   //PA15        //���ʴ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(SENSOR_B_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //���ó���������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(SENSOR_B_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
}

void JDQ_Init(void)  //�̵�����ʼ��  //PB8
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(JDQ_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=JDQ_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 // //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(JDQ_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
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
void SensorIT_PA12_Init()// �ſؿ���
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	NVIC_InitTypeDef NVIC_InitTypeDef; //NVIC 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SENSOR_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);//������˿�ʱ�Ӻ͸���ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //���ó���������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(SENSOR_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);//ӳ���ж���
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//������ʽ���� EXTI_Trigger_RisingEXTI_Trigger_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//3?��??��?D??
	
	//�ж����ȼ�
	NVIC_InitTypeDef.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitTypeDef.NVIC_IRQChannelPreemptionPriority = 0x02;                          //?��??��??��?? 2��?
	NVIC_InitTypeDef.NVIC_IRQChannelSubPriority = 0x01;																						//������??��?? 2
	NVIC_InitTypeDef.NVIC_IRQChannelCmd = ENABLE;																							//��1?����a2??D??�����̨�
	NVIC_Init(&NVIC_InitTypeDef);						
	
}
void SensorIT_PA0_Init()// �ſؿ���
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	NVIC_InitTypeDef NVIC_InitTypeDef; //NVIC 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SENSOR_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);//������˿�ʱ�Ӻ͸���ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //���ó���������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(SENSOR_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ӳ���ж���
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//������ʽ���� EXTI_Trigger_RisingEXTI_Trigger_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//3?��??��?D??
	
	//�ж����ȼ�
	NVIC_InitTypeDef.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitTypeDef.NVIC_IRQChannelPreemptionPriority = 0x02;                          //?��??��??��?? 2��?
	NVIC_InitTypeDef.NVIC_IRQChannelSubPriority = 0x01;																						//������??��?? 2
	NVIC_InitTypeDef.NVIC_IRQChannelCmd = ENABLE;																							//��1?����a2??D??�����̨�
	NVIC_Init(&NVIC_InitTypeDef);						
	
}