/*
//***********************************************  
	ʱ��			   |	����  |  ������Ŀ      |      ��� *
//***********************************************
	2018/11/20----16:29 :�޸ģ����Ӵ���1-3 �����Ӵ���������
	
	2018/12/7----16:39 :�޸ģ����Ӵ���3˫��ͨ�Ų���                   �ɹ� 

	2018/129----10:25 :�޸ģ����ӵ��������Զ��ر�TFT��

	2019/2/19----����11:14:�޸ģ����������Ƴɹ���SSP���ݴ���     �ɹ�
	
	2019/2/21----����7:12:�޸ģ��ƴμ��ĳ����ⲿ�жϣ�             �ɹ�
	2019/4/23 ----����9:35�޸ģ���������                                      �ɹ�

	2019/4/24 ----����9:35�޸ģ���������+��������                        �ɹ� �����жϻ�ȡ����ֹͣ�ߵ�ƽ �жϺ�����sensorio.c��************************************************
	2019/4/23 ----����8:23�޸ģ��޸���������                                     �޸�λ����SysTick.h�е� 
	2019/5/1 ----����9:15�޸ģ�ɾ�������ӣ�����Զ��رչ���                                     �޸�λ����SysTick.h�е� 
	2019/7/24-----------���°����������ţ������жϲ��ţ�
	
	2019/11/9-----------�޸��ظ�����---------------�ɹ�
*/
/*
	����������:
	1���ƴι��ܣ��������ʣ��Ͱڶ�������
	2�������������ܣ��������ж�
	3���Զ��ϵ繦�ܣ�����������������20s�Զ��ϵ�
*/
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "timer.h"
#include "smg4.h"
#include "MyKey.h"
#include "Usart.h"
#include "Moto.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "SensorIO.h"
#include "adc.h"
#include "sound.h"
#include "Bluetooth.h"
#include "wdg.h"
#include "semphr.h"
#define	K_PERIMETER   0.8
//#define TESTBLUE
#define _SHISHI
#define SYSTEMTIME               1//ϵͳֹͣʱ��  30x10s = 300s=5���� ��ֹͣ��ϵͳ�ϵ�
_Bool ReceOver = 0;//����1������ϱ�־

//����2����
uint8_t Usart2Data[256];
_Bool Usart2ReceOver;
//����3����
uint8_t Usart3Data[256];
_Bool Usart3ReceOver;
extern uint8_t Usart3DataLen;

//uint16_t adress = 0;//����3����tft������ַ
u16 value[3]; // ADC���ֵ����

_Bool IsPlay = 0,lastIsPlay = 0;
u8 sound_flag;
uint16_t hotdistance = 0;
u16 After_filter[3];

uint8_t SendBuffer[26] = {0}; //���ͻ��� TFT���ͻ������ݰ�


uint8_t sDat[10] = {0};
uint8_t soundNum = 0;
//extern uint8_t Freq = 0;

//�������������
uint8_t DeviceID[20] = "00010000300009000007";  //�豸��ID
uint8_t DeviceID_temp[20] = {0};  //�豸��ID
uint8_t serialdat_1[20] = {0};//�豸��
uint8_t serialdat_2[20] = {0};//�豸��
uint8_t serialdat_3[20] = {0};//����1
uint8_t serialdat_4[20] = {0};//����2
uint8_t serialdat_5[20] = {0};//����3
uint8_t serialdat_6[20] = {0};//����3
uint8_t testda[4] = {0x01,0x02,0,0};
uint16_t data_timer;//�˶�ʱ��
uint8_t senddatflag = 0;
uint8_t Freq_Heart = 0;//����
uint8_t stopflag = 0;
QueueHandle_t Key_Queue;   		//���?��?��???��?����D??����
QueueHandle_t Message_Queue;	//D??��?����D??����
//�߳̾��
TaskHandle_t task1_Handler;
TaskHandle_t task2_Handler;
TaskHandle_t task3_Handler;
void Task1(void *argument);
void Task2(void  *argument);
void Task3(void  *argument);

void SensorReData(void);
void SendDateToAndroid(int fr/*�ٶ�*/, int bmp/*����*/, int dist/*���*/,int tim/*ʱ��*/,int tur/*����*/);
int map(int x, int in_min, int in_max, int out_min, int out_max);
void Datasorting(void);//��������
SemaphoreHandle_t  xSemaphore = NULL;
void delay(u32 i)
{
	while(i--);
}

int main()
{
	delay_init();	    				//�δ�ʱ����ʼ�� 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�ж����ȼ����� ��2��
	//��ʱ����ʼ��
	TIM3_Init(1000,71-1); //��ʱ���жϳ�ʼ��,      T = 1ms    F = 1kHz
	TIM4_CH1PWM_Init(500,71);   //TIM_CH1 PWM��ʼ��
	//TIM1_Counter_Init(0xfff,3);
	//Timer1_Init();
	//���ڳ�ʼ��
	USART1_Init(57600);//�����ӿ�
	USART2_Init(9600);//���ô���
	USART3_Init(115200);//TFT��
	//��������оƬ��ʼ��
	sound_Init();
	
	//���������ų�ʼ��
	SENSOR_Init();//�ɻɹܳ�ʼ��
	SENSOR2_Init();//���ʴ�������ʼ��
	SensorIT_PB15_Init();//�ⲿ�жϳ�ʼ��
	//SensorIT_PA12_Init();
	SensorIT_PA0_Init();
	JDQ_Init();
	xSemaphore = xSemaphoreCreateBinary();
	JDQ = 0;
	//MotoInit();
	//ADC1_Init();
	//���̳߳�ʼ��
	xTaskCreate(Task1,"task1",128,NULL, 0, &task1_Handler);
	xTaskCreate(Task2,"task2",128,NULL, 1, &task2_Handler);
	xTaskCreate(Task3,"task3",128,NULL, 2, &task3_Handler);
	vTaskStartScheduler();
	IWDG_Init(4,625);    //���Ź���ʼ��	
	
	//xEventGroupCreate();
	//receiver();
	//Adc_Init();
	while(1)
	{
	
	}
}


void Task1(void *argument)   //adc����߳�
{
	int i = 0; uint16_t sys_stopTim = 0;
	//***********************ADC_DMA��ʼ��������ڴ˴�**********************
		Init_All_Periph();
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
	//*********************************************************************
	for (;;)
	{
		
	   filter();
	   for(i=0;i<3;i++)
	   {
			value[i]= GetVolt(After_filter[i]);
	   }
	  // printf("AD7 value = %d mV  \r\n",value[0]);
	   //���Դ�ӡ����
		 /* printf("AD7 value = %d mV  \r\n",value[0]);
		 printf("AD8 value = %d mV  \r\n",value[1]);
		 printf("AD9 value = %d mV  \r\n",value[2]);  */
	  
		   //SendByte2(0xaa);
		if(stopflag==1)
		{
			stopflag = 0;
			SendByte2(0xa9);
			if(sys_stopTim ++>=SYSTEMTIME)
				JDQ = 1;
		}
		vTaskDelay(10);
	}

	   
		
		
	
}
void Task2(void *argument)  //�����߳�
{
	uint8_t adress, i;

	//Message_Queue=xQueueCreate(2,50); //���?��???��Message_Queue,?����D??3��?����?��??��?����??o3???3��?��
	for (;;)
	{
		
		//SendByte2(0xab);
		//PBout(13) = 1;
		//Modebus���ݴ���
		//SendByte(value[0]);
	
		//UartASendStr("abcdefghigklmnopqrstuv",20);
		//QF_CRC(testda,4);
		//UartASendStr(testda,4);
		/* UartASendStr(serialdat_1,20);
		UartASendStr(serialdat_2,20);
		UartASendStr(serialdat_3,20);
		UartASendStr(serialdat_4,20);
		UartASendStr(serialdat_5,20); 
		UartASendStr(serialdat_6,20);  */  
		if(senddatflag==1)
		{
			senddatflag = 0;
			
				Datasorting();
		
				UartASendStr(serialdat_1,20);
				UartASendStr(serialdat_2,20);
				UartASendStr(serialdat_3,20);
				UartASendStr(serialdat_4,20);
				UartASendStr(serialdat_5,20); 
				UartASendStr(serialdat_6,20);
			
			 
		}
		 if(ReceOver)
		{
			ReceOver = 0;
			//Uart2SendData(ReceData);
			memset(ReceData,0,50);
			//ModeBusWork(ReceData,iAllReg,AllCoil);
			
		} 
		/* if(Usart3DataLen>0)
		{
			
			//Uart2SendData(Usart3Data);
		 	if ((Usart3Data[0] == 0xa5) && (Usart3Data[1] == 0x5a))
			{
				
				if (Usart3Data[2] == Usart3DataLen - 3)
				{
					adress = Usart3Data[4]; //��ַ
					switch (adress)
					{

						
					case 0x0c:  
						for (i = 6;i<Usart3DataLen;i++)
						{
							SendByte2(Usart3Data[i]);
						}
						break;
					default:
						break;
					}

				}

				Usart3DataLen = 0; 

			} 
		} */
		IWDG_Feed();
		vTaskDelay(20);
		
		//UartASendStr(ReceData,8);
	}
}
void Task3(void *argument)  //�������ź����ʴ������߳�
{	
	BaseType_t xResult;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300);
	for (;;)
	{   u8 len = 0;

		//TIM_SetCompare1(TIM4,Power);
		//SendByte2(0xac);
		//SensorReData();
	SensorReHeart();//���ʴ�����
	xResult = xSemaphoreTake(xSemaphore, (TickType_t)xMaxBlockTime);
         if(xResult == pdTRUE)
         {
				soundNum = 1;
				SendByte2(0xaf);	
				len = CalData(distance,sDat);
				SendNumber(sDat,len,1);
				sound_flag = 1; 
				data_timer = 0;
				distance = 0;
         }
	//��������˽������=============================
	/* 	if (IsPlay != lastIsPlay)
		{
			
			if(IsPlay==0)
			{
				soundNum = 1;
				SendByte2(0xaf);	
				len = CalData(distance,sDat);
				SendNumber(sDat,len,1);
				sound_flag = 1; 
				data_timer = 0;
				distance = 0;

			}
			
			delay_xms(10);
		}
		lastIsPlay = IsPlay; */
	//===========================================
	
		 vTaskDelay(5);
	}
}



void SendDateToAndroid(int fr/*�ٶ�*/, int bmp/*����*/, int dist/*���*/,int tim/*ʱ��*/,int tur/*����*/)
{
	uint8_t flg = 0;
	uint8_t h = 0,fen = 0, miao = 0;
	uint16_t destance = dist * K_PERIMETER; //�ܳ�*ת��
		if (fr > 0 && fr <2)
			flg = 1;
		else if (fr >= 2 && fr < 4)
			flg = 2;
		if (fr >= 5)
			flg = 3;
		if(fr==0)
			flg = 0;
	SendBuffer[0] = 0xA5;
	SendBuffer[1] = 0x5A;
	SendBuffer[2] = 0x17;
	SendBuffer[3] = 0x82; //дָ��
	SendBuffer[4] = 0; 
	SendBuffer[5] = 0x02;

	SendBuffer[6] = fr >> 8;  //����   ������ַ:0x0002
	SendBuffer[7] = fr & 0x00ff;

	SendBuffer[8] = bmp >> 8; //����   ������ַ:0x0003
	SendBuffer[9] = bmp & 0x00ff;

	//ʱ���֣��� ����
	h = tim / 3600;
	fen = (tim % 3600) / 60;
	miao = (tim % 3600) % 60;

	SendBuffer[10] = 0; //ʱ  0x0004-0x0006
	SendBuffer[11] = h;

	SendBuffer[12] = 0; //fen
	SendBuffer[13] = fen;

	SendBuffer[14] = 0; //miao
	SendBuffer[15] = miao;

	SendBuffer[16] = dist >> 8; //���   0x0007
	SendBuffer[17] = dist & 0x00ff;

	SendBuffer[18] = tur >> 8; //cal         0x0008
	SendBuffer[19] = tur & 0x00ff;

	SendBuffer[20] = 0; //�ٶ�ֵ   0x0009
	SendBuffer[21] = flg;

	SendBuffer[22] = 0; //����״̬   0x000A
	SendBuffer[23] = flg;
	//uint16_t Chick = CRC16(SendBuffer, 9);
	SendBuffer[24] = 0;    //����   0x000B
	SendBuffer[25] = map(value[0],0.0,3200.0,0.0,5.0);/*value[0]*/

	//Serial.write(SendBuffer, 26);//��������
	Uart3SendData(SendBuffer,26);
}
//��Χӳ�亯��-----���ڰѵ�ص�����Ч��1-10֮��
/* long map(long value, long fromStart, long fromEnd, long toStart, long toEnd)
{
	long cha = 0,shang = 0;
	cha = (value-fromStart);
	shang = (toEnd-toStart)/(fromEnd-fromStart);
	
	return cha*shang+toStart;
} */
int map(int x, int in_min, int in_max, int out_min, int out_max)
{

	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;//(val/(I_Max-I_Min)*(O_Max-O_Min) + O_Min);//
}
void Datasorting()// ��������
{
	uint8_t i = 0;

	//��3֡����
	serialdat_3[0] = 0xaa;
	serialdat_3[1] = 0x55;
	serialdat_3[2] = 0x03;
	serialdat_3[3] = data_timer >>8 ;
	serialdat_3[4] = data_timer & 0x00ff;
	serialdat_3[5] = distance >> 8;
    serialdat_3[6] = distance & 0x00ff;
	serialdat_3[7] = tmp_cal>>8;
	serialdat_3[8] = tmp_cal&0x00fff;//ת����
	serialdat_3[9] = Freq_Heart;
	serialdat_3[10] = 10;
	serialdat_3[11] = Freq >> 8;//�¶�
	serialdat_3[12] = Freq & 0x00ff;//��λ
	serialdat_3[13] = 13;//���ʸ�λ
	serialdat_3[14] = 14;//���ʵ�λ
	serialdat_3[15] = 15;//���ظ�
	serialdat_3[16] = 16;//���ص�λ
	serialdat_3[17] = 17;//У��λ
	serialdat_3[18] = 18;//У��λ
	serialdat_3[19] = 0xff;//У��λ
	QF_CRC(serialdat_3,20);
	
		//��4֡����
	serialdat_4[0] = 0xaa;
	serialdat_4[1] = 0x55;
	serialdat_4[2] = 0x04;
	serialdat_4[3] = 1;//ģʽ
	serialdat_4[4] = 2;//���ɸ�
	serialdat_4[5] = 3;//���ɵ�
    serialdat_4[6] = 4;//Ƶ��
	serialdat_4[7] = hotdistance >> 8;
	serialdat_4[8] = hotdistance & 0x00ff;//����
	serialdat_4[9] = 7;
	serialdat_4[10] = 8;//�������
	serialdat_4[11] = 9;//�������
	serialdat_4[12] = 10;
	serialdat_4[13] = ((190-Freq_Heart) *(65/100)+ 73)>>8;//�����������
	serialdat_4[14] = ((190-Freq_Heart) *(65/100)+ 73) & 0x00ff;//�����������λ
	serialdat_4[15] = 0;//��
	serialdat_4[16] = 0;//��
	serialdat_4[17] = 0;//У��λ
	serialdat_4[18] = 0;//У��λ
	QF_CRC(serialdat_4,20);
	//��5֡����
	serialdat_5[0] = 0xaa;
	serialdat_5[1] = 0x55;
	serialdat_5[2] = 0x05;
	serialdat_5[3] = 0;
	serialdat_5[4] = 0;
	serialdat_5[5] = 0;
    serialdat_5[6] = 0;
	serialdat_5[7] = 0;
	serialdat_5[8] = 0;
	serialdat_5[9] = 0;
	serialdat_5[10] = 0;
	serialdat_5[11] = 0;
	serialdat_5[12] = 0;
	serialdat_5[13] = 0;
	serialdat_5[14] = 0;
	serialdat_5[15] = 0;
	serialdat_5[16] = 0;
	serialdat_5[17] = 0;
	serialdat_5[18] = 0;
	QF_CRC(serialdat_5,20);
#ifdef TESTBLUE	
	serialdat_6[0] = 0xaa;
	serialdat_6[1] = 0x55;
	serialdat_6[2] = 0x06;
	serialdat_6[3] = 0;
	serialdat_6[4] = 0;
	serialdat_6[5] = 0;
    serialdat_6[6] = 0;
	serialdat_6[7] = 0;
	serialdat_6[8] = 0;
	serialdat_6[9] = 0;
	serialdat_6[10] = 0;
	serialdat_6[11] = 0;
	serialdat_6[12] = 0;
	serialdat_6[13] = 0;
	serialdat_6[14] = 0;
	serialdat_6[15] = 0;
	serialdat_6[16] = 0;
	serialdat_6[17] = 0;
	serialdat_6[18] = 0;
	serialdat_6[19] = 0xff;

	//�豸��Ϣaa 55 01 30 30 30 4a 53 2d 35 30 32 38 00 00 00 00 00 00 f6

	serialdat_1[0] = 0xaa;
	serialdat_1[1] = 0x55;
	serialdat_1[2] = 0x01;
	serialdat_1[3] = 0x30;
	serialdat_1[4] = 0x30;
	serialdat_1[5] = 0x30;
    serialdat_1[6] = 0x4a;
	serialdat_1[7] = 0x53;
	serialdat_1[8] = 0x2d;
	serialdat_1[9] = 0x35;
	serialdat_1[10] = 0x30;
	serialdat_1[11] = 0x32;
	serialdat_1[12] = 0x38;
	serialdat_1[13] = 0x00;
	serialdat_1[14] = 0;
	serialdat_1[15] = 0;
	serialdat_1[16] = 0;
	serialdat_1[17] = 0;
	serialdat_1[18] = 0;
	serialdat_1[19] = 0xf6;
	
	//aa 55 02 30 30 30 4a 53 2d 35 30 32 38 00 00 00 00 00 00 f6
	serialdat_2[0] = 0xaa;
	serialdat_2[1] = 0x55;
	serialdat_2[2] = 0x02;
	serialdat_2[3] = 0x30;
	serialdat_2[4] = 0x30;
	serialdat_2[5] = 0x30;
    serialdat_2[6] = 0x4a;
	serialdat_2[7] = 0x53;
	serialdat_2[8] = 0x2d;
	serialdat_2[9] = 0x35;
	serialdat_2[10] = 0x30;
	serialdat_2[11] = 0x32;
	serialdat_2[12] = 0x38;
	serialdat_2[13] = 0x00;
	serialdat_2[14] = 0;
	serialdat_2[15] = 0;
	serialdat_2[16] = 0;
	serialdat_2[17] = 0;
	serialdat_1[18] = 0;
	serialdat_2[19] = 0xf6;
#endif
#ifdef _SHISHI
	serialdat_6[0] = 0xaa;
	serialdat_6[1] = 0x55;
	serialdat_6[2] = 0x0;
	serialdat_6[3] = 0;
	serialdat_6[4] = 0;
	serialdat_6[5] = 1;
    serialdat_6[6] = 0;
	serialdat_6[7] = 0;
	serialdat_6[8] = 0;
	serialdat_6[9] = 0;
	serialdat_6[10] = 0;
	serialdat_6[11] = 0;
	serialdat_6[12] = 0;
	serialdat_6[13] = 0;
	serialdat_6[14] = 0;
	serialdat_6[15] = 0;
	serialdat_6[16] = 0;
	serialdat_6[17] = 0;
	serialdat_6[18] = 0;
	serialdat_6[19] = 0xff;

	//�豸��Ϣaa 55 01 30 30 30 4a 53 2d 35 30 32 38 00 00 00 00 00 00 f6

	serialdat_1[0] = 0xaa;
	serialdat_1[1] = 0x55;
	serialdat_1[2] = 0x01;
	serialdat_1[3] = DeviceID[0];
	serialdat_1[4] = DeviceID[1];
	serialdat_1[5] = DeviceID[2];
    serialdat_1[6] = DeviceID[3];
	serialdat_1[7] = DeviceID[4];
	serialdat_1[8] = DeviceID[5];
	serialdat_1[9] = DeviceID[6];
	serialdat_1[10] = DeviceID[7];
	serialdat_1[11] = DeviceID[8];
	serialdat_1[12] = DeviceID[9];
	serialdat_1[13] = 0;
	serialdat_1[14] = 0;
	serialdat_1[15] = 0;
	serialdat_1[16] = 0;
	serialdat_1[17] = 0;
	serialdat_1[18] = 0;
	serialdat_1[19] = 0xf6;
	QF_CRC(serialdat_1,20);
	//aa 55 02 30 30 30 4a 53 2d 35 30 32 38 00 00 00 00 00 00 f6
	serialdat_2[0] = 0xaa;
	serialdat_2[1] = 0x55;
	serialdat_2[2] = 0x02;
	serialdat_2[3] = DeviceID[10];
	serialdat_2[4] = DeviceID[11];
	serialdat_2[5] = DeviceID[12];
    serialdat_2[6] = DeviceID[13];
	serialdat_2[7] = DeviceID[14];
	serialdat_2[8] = DeviceID[15];
	serialdat_2[9] = DeviceID[16];
	serialdat_2[10] = DeviceID[17];
	serialdat_2[11] = DeviceID[18];
	serialdat_2[12] = DeviceID[19];
	serialdat_2[13] = 0x00;
	serialdat_2[14] = 0;
	serialdat_2[15] = 0;
	serialdat_2[16] = 0;
	serialdat_2[17] = 0;
	serialdat_1[18] = 0;
	serialdat_2[19] = 0xf6;
	QF_CRC(serialdat_2,20);
#endif
}