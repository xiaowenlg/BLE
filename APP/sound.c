#include "sound.h"
#include "delay.h"
uint8_t sDat_2[10] = {0};
uint8_t SDat_tim[10] = {0};
uint8_t sDat_s[10] = {0};
uint16_t distance = 0;
uint8_t TempCount = 0;
uint16_t tmp_cal = 0;
uint16_t data_timer_play = 0;
u8 first_count = 1,last_count = 0;
uint8_t len_s = 0;

void sound_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(WTN_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin= WTN_PIN ;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(WTN_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	//PB15�������
	/* GPIO_InitStructure.GPIO_Pin= GPIO_Pin_15 ;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(WTN_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */ 
	
	//GPIO_SetBits(LED_PORT, WTN_PIN );   //��LED�˿����ߣ�Ϩ������LED
}
uint8_t  Line_2A_WTN6(uint8_t SB_DATA)
{
	uint8_t S_DATA,B_DATA,j;
	CLK_2A = 1; //ʱ����
	P_DATA_2A = 1; //������
	S_DATA = SB_DATA;
	CLK_2A = 0;
	delay_xms(5);
	B_DATA = S_DATA&0X01;
	//PBout(13) = 1;
	for(j=0;j<8;j++) 
	 { 
		
		 CLK_2A = 0; // ����
		 P_DATA_2A = B_DATA; //��������һλ
		//SendByte2(B_DATA);
		 delay_us(300); //��ʱ 300us 
		 CLK_2A = 1; //����
		 delay_us(300); //��ʱ 300us 
		   
		 S_DATA = S_DATA>>1; 
		 B_DATA = S_DATA&0X01;
	 } 
	P_DATA_2A = 1; 
	CLK_2A = 1;  

	
}

//���ֶβ���
void SendNumber(uint8_t * N_Data,uint8_t len,uint8_t danwei)//����
{
	u8 i;
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(0);
	for(i = 0;i<len; i++)
	{
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(N_Data[i]);
		//SendByte2(N_Data[i]);
		
	}
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(danwei);	
	
}
void Sendtim(uint8_t * F_Data,uint8_t lenf,uint8_t *M_Data,uint8_t lenm)//ʱ��
{
	u8 i;
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(0x19);
		
		//������
	for(i = 0;i<lenf; i++)
	{
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(F_Data[i]);
		//SendByte2(N_Data[i]);
		
	}
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(0x13);	
//����
	for(i = 0;i<lenm; i++)
	{
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(M_Data[i]);
		//SendByte2(N_Data[i]);
		
	}
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(0x14);
	
}
void SendNumber_hot(uint8_t * N_Data,uint8_t len)
{
	u8 i;
	//while(!WTN_BUSY);
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(2);//��������
	for(i = 0;i<len; i++)
	{
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(N_Data[i]);
		//SendByte2(N_Data[i]);
		
	}
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(3);	
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(4);//��ӭ�ٴ�ʹ��
	
}
uint8_t CalData(uint16_t dat,uint8_t *res_dat)// ������ַ�ֽ����
{
	uint8_t len, q, b, s, g;
	if(dat>9999)
		dat = 9999;
	q = dat / 1000; //ǧλ
	b = dat % 1000 / 100;//��λ
	s = dat % 100 / 10; //ʮλ
	g = dat % 10;  //��λ
	
	if((dat<=9)&&(dat>=0)) //0-9
	{
		res_dat[0] = dat+5;//������ַ5-14
		len = 1;
	}
	if((dat>9)&&(dat<=99))//10-99
	{
		if((dat%10)==0)
		{
			res_dat[0] = s+5;
			res_dat[1] = 15;//������ַ15 ��ʮ
			len = 2;
		}
		else
		{
			res_dat[0] = s+5;
			res_dat[1] = 15;
			res_dat[2] = g+5;
			len = 3;
		}
	}
	if((dat>99)&&(dat<=999))//100-999
	{
		if((dat%100)==0)
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			len = 2;
		}
		else if ((dat%10)==0)
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			res_dat[2] = s+5;
			res_dat[3] = 15;//������ַ15 ��ʮ
			len = 4;
		}
		else if(dat%100>0&&dat%100<10)
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			res_dat[2] = 5;
			res_dat[3] = g+5;
			len = 4;
				 
		}
		else
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			res_dat[2] = s+5;
			res_dat[3] = 15;//������ַ15 ��ʮ
			res_dat[4] = g+5;
			len = 5;
		}
		
	}
	if((dat>999)&&(dat<=9999))//1000-9999
		{
			if((dat%1000)==0)
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ17 ��ǧλ
				len = 2;
			}
			else if(b==0&&s==0&&g!=0)//001
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ16 ��ǧλ
				res_dat[2] = 5;
				res_dat[3] = g+5;
				len = 4;
			}
			else if(b==0&&s!=0&&g==0)//010
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ16 ��ǧλ
				res_dat[2] = 5;
				res_dat[3] = s+5;//������ַ15 ��ʮ
				res_dat[4] = 15;
				len = 5;
			}
			else if(b==0&&s!=0&&g!=0)//011--------��λ=0 ʮλ�͸�λ������0----------------��Ϊ�˴����ж�һ��״̬�������ظ�����
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;
				res_dat[2] = 5;
				res_dat[3] = s+5;//������ַ15 ��ʮ
				res_dat[4] = 15;
				res_dat[5] = g+5;
				len = 6;
				
			}
			else if(b!=0&&s==0&&g==0)//100
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ16 ��ǧλ
				res_dat[2] = b+5;//������ַ15 ��ʮ
				res_dat[3] = 16;
				len = 4;
			}
			 else if(b!=0&&s!=0&&g==0)//110*********************************
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ16 ��ǧλ
				res_dat[2] = b+5;//������ַ15 ��ʮ
				res_dat[3] = 16;
				res_dat[4] = s+5;
				res_dat[5] = 15;
				len = 6;
			} 
			 else if(b!=0&&s==0&&g!=0)//101
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ16 ��ǧλ
				res_dat[2] = b+5;//������ַ15 ��ʮ
				res_dat[3] = 16;
				res_dat[4] = 5;
				res_dat[5] = g+5;
				len = 6;
			} 
			//else if(b!=0&&s!=0&&g!=0)//111
			else//111
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//������ַ16 ��ǧλ
				res_dat[2] = b+5;
				res_dat[3] = 16;
				res_dat[4] = s+5;
				res_dat[5] = 15;
				res_dat[6] = g+5;
				len = 7;
			}
			
			
		}
	
	return len;//�������鳤��
}

//���ų�������
void PlaySound(uint8_t C_Data)
{
	
	Line_2A_WTN6(C_Data);
	while(!WTN_BUSY);
	
}
//�ⲿ�жϲɼ�����
void SensorIT_PB15_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	NVIC_InitTypeDef NVIC_InitTypeDef; //NVIC 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(WTN_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);//������˿�ʱ�Ӻ͸���ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //���ó���������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init( WTN_PORT ,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);//ӳ���ж���
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//������ʽ���� EXTI_Trigger_Rising
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//3?��??��?D??
	
	//�ж����ȼ�
	NVIC_InitTypeDef.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitTypeDef.NVIC_IRQChannelPreemptionPriority = 0x02;                          //?��??��??��?? 2��?
	NVIC_InitTypeDef.NVIC_IRQChannelSubPriority = 0x02;																						//������??��?? 2
	NVIC_InitTypeDef.NVIC_IRQChannelCmd = ENABLE;																							//��1?����a2??D??�����̨�
	NVIC_Init(&NVIC_InitTypeDef);						
	
}
void EXTI15_10_IRQHandler()
{ 
	
		if(EXTI_GetITStatus(EXTI_Line15)!= RESET)
		{
			uint8_t len = 0;
				delay_xms(10);
				if(WTN_BUSY==1)
				{
					///soundNum++;
					if(sound_flag==1)
					{
						switch (soundNum)
						{
							case 1: 
								len = CalData(data_timer_play/60,SDat_tim);
								len_s = CalData(data_timer_play%60,sDat_s);
								Sendtim(SDat_tim,len,sDat_s,len_s);
								soundNum = 2;
								break;
							case 2:
								len = CalData(tmp_cal,sDat_2);
								SendNumber_hot(sDat_2,len);
								sound_flag = 0;
								hotdistance = 0;
								data_timer_play = 0;
								break;
						default:
							break;
						}
						
					//SendByte2(soundNum);//������
						
						
					}
				}
				
				/* if(WTN_BUSY==1)
				{
				
					if(sound_flag==1)
					{
						sound_flag = 0;
						
						len = CalData(9999,SDat_tim);
						Sendtim(SDat_tim,len,0x13);
						len = CalData(1234,sDat_2);
						SendNumber_hot(sDat_2,len);
					}
					 //SendByte2(0xaa);
					soundNum++;
					 //SendByte2(soundNum);
				}
				 */
		}
		if(EXTI_GetITStatus(EXTI_Line12)!= RESET)
		{
			
				delay_xms(10);
				if(SENSOR_1==0)
				{
					
					
					TempCount++; 
					distance++;
					hotdistance++;
					//SendByte2(distance);
				}
				 
				/* first_count = SENSOR_1 ;
				if (first_count != last_count)
				{
					if(first_count==0)
					{
						TempCount++; 
						distance++;
						hotdistance++;
					}
				}
				last_count = first_count; */
				
					
					
					
					//SendByte2(distance);
				
		}
		
		EXTI_ClearITPendingBit(EXTI_Line15);   //??3y LINE ��?��??D??����????
		EXTI_ClearITPendingBit(EXTI_Line12); 
}

void EXTI0_IRQHandler()
{ 
	
	
		if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
		{
			
				delay_xms(10);
				if(SENSOR_1==0)
				{
					//tim_jdq = 0;
					TempCount++; 
					distance++;
					hotdistance++;
					//distance = 1234;
					//hotdistance = distance;
				}
				 
				
			EXTI_ClearITPendingBit(EXTI_Line0); 	
		}
		
		
		
}
