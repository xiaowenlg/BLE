
/*//////////////////��ռ�ڴ�İ�������///////////////////////////
*
*     �Լ���д��ֲ�İ�������
*
*/////////////////////////////////////////////////////////////
#include "MyKey.h"

//���峤������TICK��,�Լ���_�������TICK��
#define	 KEY_LONG_PERIOD         100
#define	 KEY_CONTINUE_PERIOD     1

   

 //���尴��״̬
#define	KEY_STATE_INIT            		0
#define	KEY_STATE_WOBBLE            	1
#define	KEY_STATE_PRESS            	2
#define	KEY_STATE_LONG            		3
#define	KEY_STATE_CONTINUE       	4
#define 	KEY_STATE_RELEASE         	5


//�����ӿڶ���
#define IO_KEY_1         PEin(0)
#define IO_KEY_2         PEin(1)
#define IO_KEY_3         PEin(2)
#define IO_KEY_4         PEin(3)
#define IO_KEY_5         PEin(4)
#define IO_KEY_6         PEin(5)
#define IO_KEY_7         PEin(6)

//�����˿�PE�ڳ�ʼ��
#define IO_KEY_CLOCK     RCC_APB2Periph_GPIOC   //ʱ��
#define IO_KEY_PIN       (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)  //����
#define IO_KEY_PORT      GPIOE                 //�˿�

//IO�ڳ�ʼ��
void IO_KEY_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = IO_KEY_PIN;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //������������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
		GPIO_Init(IO_KEY_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
}
//��ʼ��
void KeyInit(void)
{
		IO_KEY_1 = 1;
		IO_KEY_2 = 1;
		IO_KEY_3 = 1;
		IO_KEY_4 = 1;
		IO_KEY_5 = 1;
		IO_KEY_6 = 1;
		
}

void Key_All_Init(void)
{
	IO_KEY_Init();
	KeyInit();

}
//����ɨ��
uint16_t KeyScan(void)
{

		if(IO_KEY_1==0)  return KEY_VALUE_1 ; 
		if(IO_KEY_2==0)  return KEY_VALUE_2 ;
		if(IO_KEY_3==0)  return KEY_VALUE_3 ;
		if(IO_KEY_4==0)  return KEY_VALUE_4 ;
		if(IO_KEY_5==0)  return KEY_VALUE_5 ;
		if(IO_KEY_6==0)  return KEY_VALUE_6 ;
		return KEY_NULL;
}

void KeyLoop(uint16_t *pKeyValue)
{

		static uint8_t s_u8KeyState = KEY_STATE_INIT ;
     static uint8_t s_u8KeyTimeCount = 0 ;
     static uint16_t s_u8LastKey = KEY_NULL ;   //���水���ͷ�ʱ��ļ�ֵ
     uint16_t KeyTemp = KEY_NULL ;

     KeyTemp = KeyScan();         //��ȡ��ֵ
     switch(s_u8KeyState)
     {
         case KEY_STATE_INIT :
                {
                    if(KEY_NULL != (KeyTemp))
                    {
                        s_u8KeyState = KEY_STATE_WOBBLE ;
                    }
                }
         break ;
         case KEY_STATE_WOBBLE:       //����
                {
                    s_u8KeyState = KEY_STATE_PRESS ;     
                }
         break ;
         case KEY_STATE_PRESS :
                {
                    if(KEY_NULL != (KeyTemp))
                    {
                        s_u8LastKey = KeyTemp ; //�����ֵ,�Ա����ͷŰ���״̬���ؼ�ֵ
                        KeyTemp |= KEY_DOWN ;   //��������
                        s_u8KeyState = KEY_STATE_LONG ;
                    }
                    else
                    {
                        s_u8KeyState = KEY_STATE_INIT ;
                    }
                }
         break ;
         case KEY_STATE_LONG :
                {
                    if(KEY_NULL != (KeyTemp))
                    {
                        if(++s_u8KeyTimeCount > KEY_LONG_PERIOD)
                        {
                            s_u8KeyTimeCount = 0 ;
                            KeyTemp |= KEY_LONG ;   //�������¼�����
                            s_u8KeyState = KEY_STATE_CONTINUE ;
                        }
                    }
                    else
                    {
                        s_u8KeyState = KEY_STATE_RELEASE ;
                    }
                }
         break ;
         case KEY_STATE_CONTINUE :
                {
                    if(KEY_NULL != (KeyTemp))
                    {
                        if(++s_u8KeyTimeCount > KEY_CONTINUE_PERIOD)
                        {
                            s_u8KeyTimeCount = 0 ;
                            KeyTemp |= KEY_CONTINUE ;
                        }
                    }
                    else
                    {
                        s_u8KeyState = KEY_STATE_RELEASE ;
                    }
                }
         break ;
         case KEY_STATE_RELEASE :
                {
                    s_u8LastKey |= KEY_UP ;
                    KeyTemp = s_u8LastKey ;
                    s_u8KeyState = KEY_STATE_INIT ;
                }
         break ;
         default : break ;
     }
     *pKeyValue = KeyTemp ; //���ؼ�ֵ   
}
