#include "Moto.h"

u8 CCW[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};   //逆时钟旋转相序表
u8 CW[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};    //正时钟旋转相序表
u8 double_pos[4]={0x06,0x03,0x09,0x0c};//双四拍驱动方式正转表 AD-DC-CB-BA
u8  double_rev[4]={0x0c,0x09,0x03,0x06};//双四拍驱动方式反转表 AB-BC-CD-DA


u8  double_rev_1[4]={0x0e,0x0d,0x0b,0x07};//双四拍驱动方式反转表 AB-BC-CD-DA
void MotoInit()  //端口初始化
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_MOTO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = MOTO_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTO_Port,&GPIO_InitStruct);
	
}
void MotoCw(u16 n)//步进控制
{	u16 k = 0;
	for(k=0;k<n;k++)
    {
		u8 i=0;
		for (i=0;i<4;i++)//在八拍工作方式下，一个脉冲转子转动角度为5.625度，八拍共45度
		{
			GPIO_Write(MOTO_Port,double_pos[i]);
			delay_ms(10);
			i++;
		}
	}
}
