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
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(WTN_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin= WTN_PIN ;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(WTN_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	//PB15设成输入
	/* GPIO_InitStructure.GPIO_Pin= GPIO_Pin_15 ;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(WTN_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */ 
	
	//GPIO_SetBits(LED_PORT, WTN_PIN );   //将LED端口拉高，熄灭所有LED
}
uint8_t  Line_2A_WTN6(uint8_t SB_DATA)
{
	uint8_t S_DATA,B_DATA,j;
	CLK_2A = 1; //时钟线
	P_DATA_2A = 1; //数据线
	S_DATA = SB_DATA;
	CLK_2A = 0;
	delay_xms(5);
	B_DATA = S_DATA&0X01;
	//PBout(13) = 1;
	for(j=0;j<8;j++) 
	 { 
		
		 CLK_2A = 0; // 拉低
		 P_DATA_2A = B_DATA; //传输数据一位
		//SendByte2(B_DATA);
		 delay_us(300); //延时 300us 
		 CLK_2A = 1; //拉高
		 delay_us(300); //延时 300us 
		   
		 S_DATA = S_DATA>>1; 
		 B_DATA = S_DATA&0X01;
	 } 
	P_DATA_2A = 1; 
	CLK_2A = 1;  

	
}

//数字段播放
void SendNumber(uint8_t * N_Data,uint8_t len,uint8_t danwei)//次数
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
void Sendtim(uint8_t * F_Data,uint8_t lenf,uint8_t *M_Data,uint8_t lenm)//时间
{
	u8 i;
		Line_2A_WTN6(0xf3);
		delay_xms(2);
		Line_2A_WTN6(0x19);
		
		//报分钟
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
//报秒
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
		Line_2A_WTN6(2);//消耗热量
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
		Line_2A_WTN6(4);//欢迎再次使用
	
}
uint8_t CalData(uint16_t dat,uint8_t *res_dat)// 语音地址分解计算
{
	uint8_t len, q, b, s, g;
	if(dat>9999)
		dat = 9999;
	q = dat / 1000; //千位
	b = dat % 1000 / 100;//百位
	s = dat % 100 / 10; //十位
	g = dat % 10;  //个位
	
	if((dat<=9)&&(dat>=0)) //0-9
	{
		res_dat[0] = dat+5;//语音地址5-14
		len = 1;
	}
	if((dat>9)&&(dat<=99))//10-99
	{
		if((dat%10)==0)
		{
			res_dat[0] = s+5;
			res_dat[1] = 15;//语音地址15 是十
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
			res_dat[1] = 16;//语音地址16 是百
			len = 2;
		}
		else if ((dat%10)==0)
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//语音地址16 是百
			res_dat[2] = s+5;
			res_dat[3] = 15;//语音地址15 是十
			len = 4;
		}
		else if(dat%100>0&&dat%100<10)
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//语音地址16 是百
			res_dat[2] = 5;
			res_dat[3] = g+5;
			len = 4;
				 
		}
		else
		{
			res_dat[0] = b+5;
			res_dat[1] = 16;//语音地址16 是百
			res_dat[2] = s+5;
			res_dat[3] = 15;//语音地址15 是十
			res_dat[4] = g+5;
			len = 5;
		}
		
	}
	if((dat>999)&&(dat<=9999))//1000-9999
		{
			if((dat%1000)==0)
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址17 是千位
				len = 2;
			}
			else if(b==0&&s==0&&g!=0)//001
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址16 是千位
				res_dat[2] = 5;
				res_dat[3] = g+5;
				len = 4;
			}
			else if(b==0&&s!=0&&g==0)//010
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址16 是千位
				res_dat[2] = 5;
				res_dat[3] = s+5;//语音地址15 是十
				res_dat[4] = 15;
				len = 5;
			}
			else if(b==0&&s!=0&&g!=0)//011--------百位=0 十位和个位不等于0----------------因为此处少判断一种状态，导致重复播报
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;
				res_dat[2] = 5;
				res_dat[3] = s+5;//语音地址15 是十
				res_dat[4] = 15;
				res_dat[5] = g+5;
				len = 6;
				
			}
			else if(b!=0&&s==0&&g==0)//100
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址16 是千位
				res_dat[2] = b+5;//语音地址15 是十
				res_dat[3] = 16;
				len = 4;
			}
			 else if(b!=0&&s!=0&&g==0)//110*********************************
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址16 是千位
				res_dat[2] = b+5;//语音地址15 是十
				res_dat[3] = 16;
				res_dat[4] = s+5;
				res_dat[5] = 15;
				len = 6;
			} 
			 else if(b!=0&&s==0&&g!=0)//101
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址16 是千位
				res_dat[2] = b+5;//语音地址15 是十
				res_dat[3] = 16;
				res_dat[4] = 5;
				res_dat[5] = g+5;
				len = 6;
			} 
			//else if(b!=0&&s!=0&&g!=0)//111
			else//111
			{
				res_dat[0] = q+5;
				res_dat[1] = 17;//语音地址16 是千位
				res_dat[2] = b+5;
				res_dat[3] = 16;
				res_dat[4] = s+5;
				res_dat[5] = 15;
				res_dat[6] = g+5;
				len = 7;
			}
			
			
		}
	
	return len;//返回数组长度
}

//播放常规语音
void PlaySound(uint8_t C_Data)
{
	
	Line_2A_WTN6(C_Data);
	while(!WTN_BUSY);
	
}
//外部中断采集数据
void SensorIT_PB15_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	NVIC_InitTypeDef NVIC_InitTypeDef; //NVIC 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(WTN_PORT_RCC|RCC_APB2Periph_AFIO,ENABLE);//开外设端口时钟和复用时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //设置成上拉输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init( WTN_PORT ,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);//映射中断线
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//触发方式配置 EXTI_Trigger_Rising
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//3?ê??ˉ?D??
	
	//中断优先级
	NVIC_InitTypeDef.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitTypeDef.NVIC_IRQChannelPreemptionPriority = 0x02;                          //?à??ó??è?? 2￡?
	NVIC_InitTypeDef.NVIC_IRQChannelSubPriority = 0x02;																						//×óó??è?? 2
	NVIC_InitTypeDef.NVIC_IRQChannelCmd = ENABLE;																							//ê1?üía2??D??í¨μà
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
						
					//SendByte2(soundNum);//测试用
						
						
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
		
		EXTI_ClearITPendingBit(EXTI_Line15);   //??3y LINE é?μ??D??±ê????
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
