#include "Bluetooth.h"
#include "Usart.h"

/*******************************************************************************
����ͨ��Э��
//У�鷽ʽ���԰�����1֡��֡ͷ������18֡��������ֽ���Կ�İ�λ�����Կ�ݶ�Ϊ0XE1��0XE2.
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

