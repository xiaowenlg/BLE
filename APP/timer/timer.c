/*
	��ʱ����ʼ�����ж�

*/


#include "timer.h"
#include "stm32f10x.h"
#include  "led.h"
#include "smg4.h"
#include "Usart.h"
#include "SensorIO.h"
#include "sound.h"

//���� 10cal/t
//���� 8
//������ 4
//������  6

// ̫������ 1
//Ť���� 2 
//�Ӱ��� 7

#define ALPHA 				6//��������ϵ��
uint8_t Freq = 0;
//uint8_t Freq_Heart = 0;
uint8_t button1[6] = {0xA5,0x5A,0x03,0x80,0x4F,0x01};
uint8_t GetTime[6] = {0xA5, 0x5A, 0x03, 0x81, 0x0c, 0x04}; //��ȡ��ǰʱ��
typedef uint32_t ULONG;
int nowtime = 0;
uint16_t tim = 0, lasttime = 0;
//uint16_t tmp_cal = 0;
_Bool invar = 0,lastvar = 0;
void TIM3_Init(u16 per,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //��ʱ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;														//�ж����ȼ��ṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //1.ʹ�ܶ�ʱ��ʱ��
	
	//2.��ʼ����ʱ������,�����Զ���װֵ�� ��Ƶϵ���� ������ʽ��
	TIM_TimeBaseInitStructure.TIM_Period=per; //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//3.���ö�ʱ���ж����ͣ� ��ʹ��
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //������ʱ���ж�
	//���Ȱ��жϱ�־λ���
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	//4.�ж����ȼ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;// �� ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	//5.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��
}
void TIM2_Init(ULONG ulBaudRate)
{
	ULONG           usTimerT35_50us;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //��ʱ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;														//�ж����ȼ��ṹ��
	uint16_t PrescalerValue = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);        //1.ʹ�ܶ�ʱ��ʱ��
	
	usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
	PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
	//2.��ʼ����ʱ������,�����Զ���װֵ�� ��Ƶϵ���� ������ʽ��
	TIM_TimeBaseInitStructure.TIM_Period=(uint16_t) usTimerT35_50us;//�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=PrescalerValue; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//3.���ö�ʱ���ж����ͣ� ��ʹ��
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //������ʱ���ж�
	//���Ȱ��жϱ�־λ���
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	//4.�ж����ȼ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;// �� ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	//5.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��
}
void Timer1_Init()   //����Timer1Ϊ������
{
	//NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	


	/* NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ��� */
   //����TIMER1��Ϊ������
   TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // Time base configuration


	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1, ENABLE);
	
}
void TIM1_Counter_Init(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructuer;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///ʹ��TIM1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructuer.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructuer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructuer.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructuer);
	//timer1 NVIC ����

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	  TIM_TimeBaseInitStructure.TIM_Period = arr; //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted,0);//5�β����˲�   �ⲿʱ��ģʽ2
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ�������һ���Զ������ж�һ��
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

	TIM_SetCounter(TIM1,0);//���ü�����ֵ
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
}
void TIM3_IRQHandler(void)
{
	static uint16_t timer_count = 0,tim_jdq = 0,Timer_Shin = 0,timer = 0 /*u16 timer = 0,,Timer_led = 0*/ ,soundtim = 0;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
		if(TIM_GetITStatus(TIM3,TIM_IT_Update))
		{
			 if(Timer_Shin++ >100)//��ʱ��TFT����������
			 {
				 Timer_Shin = 0;
				 if(Freq !=0)
				 {
					  
					 tmp_cal  = hotdistance *ALPHA;
					SendDateToAndroid(Freq,Freq_Heart,hotdistance,data_timer,tmp_cal);	//TFT�����ݴ���
					Uart3SendData(button1,6);
				 }
				
				senddatflag = 1;
			 }
			if (timer_count++>1000) //����������ʼƴ�
			{

				timer_count = 0;
				Freq = TempCount;
				TempCount = 0;
				//����
				Freq_Heart = TempCount_Heart;
				TempCount_Heart = 0;
				//�˶�ʱ��
				//���Դ���---------------------------
				 //SendByte2(0x05);
				 //SendByte2(Freq);
				// SendByte2(distance);
				//*------------------------------------
				
				if(Freq != 0)
				{
					timer++;
					data_timer++;
					data_timer_play++;
					//data_timer_play = 255;
					//data_timer = 255;
				}
				//��������
				/* nowtime = timer- lasttime;
				tmp_cal = tmp_cal + (Power *nowtime*ALPHA); //��������
				lasttime = timer; */
			}
			if (Freq==0) //��ʱ����
			{
				 tim_jdq++;
					if (tim_jdq>=6000)//��ʱ5s�󲥱�
					{
						
						tim_jdq = 0;
						//IsPlay = 0;
						invar = 1;

						 
					} 
					if (WTN_BUSY==1&&invar==1&&soundNum==2)
					{
							soundtim++;
							
						if (soundtim>10000)
						{		
								stopflag = 1;
								soundtim = 0;
								timer = 0;
						}
					}
					else
						soundtim = 0;
			}
			
			else
				{
					JDQ = 0;
					tim_jdq = 0;
					//IsPlay = 1;
					invar = 0;
				}
			
			if (invar != lastvar)
			{
				if (invar == 1)
				{
					xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken)
				}
				//delay_xms(10);
			}
			lastvar = invar;
		} 
		 TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
	  
	//Modbus_State = 0;
	ReceOver = 1;
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

 
  }
}
void TIM1_UP_IRQHandler(void)//��ʱ��1�����ж�
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
	{
		
		//printf("\r\n*********���ټ������***********\r\n");
		//SendByte2(11);
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
}
