/*
	定时器初始化，中断

*/


#include "timer.h"
#include "stm32f10x.h"
#include  "led.h"
#include "smg4.h"
#include "Usart.h"
#include "SensorIO.h"
#include "sound.h"

//坐推 10cal/t
//健身车 8
//漫步机 4
//蹬力器  6

// 太极揉腿 1
//扭腿器 2 
//钟摆器 7

#define ALPHA 				6//消耗热量系数
uint8_t Freq = 0;
//uint8_t Freq_Heart = 0;
uint8_t button1[6] = {0xA5,0x5A,0x03,0x80,0x4F,0x01};
uint8_t GetTime[6] = {0xA5, 0x5A, 0x03, 0x81, 0x0c, 0x04}; //读取当前时间
typedef uint32_t ULONG;
int nowtime = 0;
uint16_t tim = 0, lasttime = 0;
//uint16_t tmp_cal = 0;
_Bool invar = 0,lastvar = 0;
void TIM3_Init(u16 per,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //定时器结构体
	NVIC_InitTypeDef NVIC_InitStructure;														//中断优先级结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //1.使能定时器时钟
	
	//2.初始化定时器参数,包含自动重装值， 分频系数， 计数方式等
	TIM_TimeBaseInitStructure.TIM_Period=per; //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//设置向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//3.设置定时器中断类型， 并使能
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //开启定时器中断
	//首先把中断标志位清除
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	//4.中断优先级别初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;// 抢 占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	//5.使能定时器
	TIM_Cmd(TIM3,ENABLE); //使能定时器
}
void TIM2_Init(ULONG ulBaudRate)
{
	ULONG           usTimerT35_50us;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //定时器结构体
	NVIC_InitTypeDef NVIC_InitStructure;														//中断优先级结构体
	uint16_t PrescalerValue = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);        //1.使能定时器时钟
	
	usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
	PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
	//2.初始化定时器参数,包含自动重装值， 分频系数， 计数方式等
	TIM_TimeBaseInitStructure.TIM_Period=(uint16_t) usTimerT35_50us;//自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=PrescalerValue; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//设置向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//3.设置定时器中断类型， 并使能
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //开启定时器中断
	//首先把中断标志位清除
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	//4.中断优先级别初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;// 抢 占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	//5.使能定时器
	TIM_Cmd(TIM2,ENABLE); //使能定时器
}
void Timer1_Init()   //配置Timer1为计数器
{
	//NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	


	/* NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器 */
   //配置TIMER1作为计数器
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///使能TIM1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructuer.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructuer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructuer.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructuer);
	//timer1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
	  TIM_TimeBaseInitStructure.TIM_Period = arr; //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1
	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted,0);//5次采样滤波   外部时钟模式2
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位，避免第一次自动进入中断一次
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

	TIM_SetCounter(TIM1,0);//设置计数初值
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
}
void TIM3_IRQHandler(void)
{
	static uint16_t timer_count = 0,tim_jdq = 0,Timer_Shin = 0,timer = 0 /*u16 timer = 0,,Timer_led = 0*/ ,soundtim = 0;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
		if(TIM_GetITStatus(TIM3,TIM_IT_Update))
		{
			 if(Timer_Shin++ >100)//定时向TFT屏发送数据
			 {
				 Timer_Shin = 0;
				 if(Freq !=0)
				 {
					  
					 tmp_cal  = hotdistance *ALPHA;
					SendDateToAndroid(Freq,Freq_Heart,hotdistance,data_timer,tmp_cal);	//TFT屏数据传输
					Uart3SendData(button1,6);
				 }
				
				senddatflag = 1;
			 }
			if (timer_count++>1000) //活动次数和心率计次
			{

				timer_count = 0;
				Freq = TempCount;
				TempCount = 0;
				//心率
				Freq_Heart = TempCount_Heart;
				TempCount_Heart = 0;
				//运动时间
				//测试代码---------------------------
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
				//消耗热量
				/* nowtime = timer- lasttime;
				tmp_cal = tmp_cal + (Power *nowtime*ALPHA); //消耗热量
				lasttime = timer; */
			}
			if (Freq==0) //延时播报
			{
				 tim_jdq++;
					if (tim_jdq>=6000)//延时5s后播报
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
void TIM1_UP_IRQHandler(void)//定时器1更新中断
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
	{
		
		//printf("\r\n*********测速计数溢出***********\r\n");
		//SendByte2(11);
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}
