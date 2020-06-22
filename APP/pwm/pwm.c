
/*******************************************************************************
PWM输出初始化
*******************************************************************************/

#include "pwm.h"

/*******************************************************************************
* 函 数 名 : TIM4_CH1PWM_Init
* 函数功能: TIM4通道1PWM初始化
* 输    入 : arr：自动重装载值  psc：预分频系数
* 输    出         : 无
*******************************************************************************/
void TIM4_CH1PWM_Init(u16 per,u16 psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //定时器结构体
		TIM_OCInitTypeDef       TIM_OCInitStructure;
	
		RCC_APB2PeriphClockCmd(PWM_PART_RCC,ENABLE);                   //打开端口和复用功能时钟
		RCC_APB1PeriphClockCmd(PWM_TIM_RCC,ENABLE);											//使能Timer3时钟
	
		GPIO_InitStructure.GPIO_Pin = PWM_PIN;                           //  GPIO初始化结构体
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PWM_PART,&GPIO_InitStructure);
	
	
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //初始化Timer3
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_Period = per;
		TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
		TIM_TimeBaseInit(PWM_TIM,&TIM_TimeBaseInitStructure);
	
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1 ;                         //PWM结构体初始化
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
		TIM_OCInitStructure.TIM_Pulse = 0;//初始占空比为0
		TIM_OC1Init(PWM_TIM,&TIM_OCInitStructure);
		
		
		//GPIO_PinRemapConfig(GPIO_FullRemap_TIM4,ENABLE);                //输出IO口重映射
		
		TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);          //使能预装载存储器
		TIM_ARRPreloadConfig(TIM4,ENABLE);												//使能定时器预装载存储器
		TIM_Cmd(TIM4,ENABLE);																			//打开定时器
	
}
