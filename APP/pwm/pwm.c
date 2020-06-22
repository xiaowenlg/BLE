
/*******************************************************************************
PWM�����ʼ��
*******************************************************************************/

#include "pwm.h"

/*******************************************************************************
* �� �� �� : TIM4_CH1PWM_Init
* ��������: TIM4ͨ��1PWM��ʼ��
* ��    �� : arr���Զ���װ��ֵ  psc��Ԥ��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_CH1PWM_Init(u16 per,u16 psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //��ʱ���ṹ��
		TIM_OCInitTypeDef       TIM_OCInitStructure;
	
		RCC_APB2PeriphClockCmd(PWM_PART_RCC,ENABLE);                   //�򿪶˿ں͸��ù���ʱ��
		RCC_APB1PeriphClockCmd(PWM_TIM_RCC,ENABLE);											//ʹ��Timer3ʱ��
	
		GPIO_InitStructure.GPIO_Pin = PWM_PIN;                           //  GPIO��ʼ���ṹ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PWM_PART,&GPIO_InitStructure);
	
	
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //��ʼ��Timer3
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_Period = per;
		TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
		TIM_TimeBaseInit(PWM_TIM,&TIM_TimeBaseInitStructure);
	
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1 ;                         //PWM�ṹ���ʼ��
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
		TIM_OCInitStructure.TIM_Pulse = 0;//��ʼռ�ձ�Ϊ0
		TIM_OC1Init(PWM_TIM,&TIM_OCInitStructure);
		
		
		//GPIO_PinRemapConfig(GPIO_FullRemap_TIM4,ENABLE);                //���IO����ӳ��
		
		TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);          //ʹ��Ԥװ�ش洢��
		TIM_ARRPreloadConfig(TIM4,ENABLE);												//ʹ�ܶ�ʱ��Ԥװ�ش洢��
		TIM_Cmd(TIM4,ENABLE);																			//�򿪶�ʱ��
	
}
