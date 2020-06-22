#include "Bluetooth.h"
#include "Usart.h"

/*******************************************************************************
蓝牙通信协议
//校验方式：自包括第1帧（帧头）到第18帧外加两个字节秘钥的按位异或，秘钥暂定为0XE1，0XE2.
*******************************************************************************/
void  QF_CRC (uint8_t *dat,uint8_t len)
{
	uint8_t crctemp_lo = 0,crctemp_hi,i = 0;
	//SendByte(0xaa);
	for(i=0;i<len-2;i++)
	{
		if(i==0)
		{
			crctemp_hi = dat[i]^0xe1;
			
			crctemp_lo = dat[i]^0xe2;
		}

		else
		{
			crctemp_hi = crctemp_hi^dat[i];
			crctemp_lo = crctemp_lo^dat[i];
		}
		
	}
	//SendByte(crctemp_hi);
	dat[18] = crctemp_hi;
	dat[19] = crctemp_lo;
}

