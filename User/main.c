/*
//***********************************************  
	时间			   |	操作  |  操作项目      |      结果 *
//***********************************************
	2018/11/20----16:29 :修改，增加串口1-3 ，增加次数检测程序
	
	2018/12/7----16:39 :修改，增加串口3双向通信测试                   成功 

	2018/129----10:25 :修改，增加电量测试自动关闭TFT屏

	2019/2/19----下午11:14:修改，蓝牙改名称成功，SSP数据传输     成功
	
	2019/2/21----下午7:12:修改，计次检查改成了外部中断，             成功
	2019/4/23 ----下午9:35修改，增加语音                                      成功

	2019/4/24 ----下午9:35修改，增加语音+播报数字                        成功 利用中断获取播放停止高电平 中断函数在sensorio.c中************************************************
	2019/4/23 ----下午8:23修改，修改死机现象                                     修改位置在SysTick.h中的 
	2019/5/1 ----下午9:15修改，删除无用杂，添加自动关闭功能                                     修改位置在SysTick.h中的 
	2019/7/24-----------改新板后加语音播放（利用中断播放）
	
	2019/11/9-----------修改重复播报---------------成功
*/
/*
	程序功能描述:
	1，计次功能（包括心率，和摆动次数）
	2，语音播报功能，利用了中断
	3，自动断电功能，在语音播报结束后20s自动断电
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
#define SYSTEMTIME               1//系统停止时间  30x10s = 300s=5分钟 后停止，系统断电
_Bool ReceOver = 0;//串口1接收完毕标志

//串口2变量
uint8_t Usart2Data[256];
_Bool Usart2ReceOver;
//串口3变量
uint8_t Usart3Data[256];
_Bool Usart3ReceOver;
extern uint8_t Usart3DataLen;

//uint16_t adress = 0;//串口3接收tft屏数地址
u16 value[3]; // ADC检测值保存

_Bool IsPlay = 0,lastIsPlay = 0;
u8 sound_flag;
uint16_t hotdistance = 0;
u16 After_filter[3];

uint8_t SendBuffer[26] = {0}; //发送缓存 TFT发送缓存数据包


uint8_t sDat[10] = {0};
uint8_t soundNum = 0;
//extern uint8_t Freq = 0;

//蓝牙传输的数据
uint8_t DeviceID[20] = "00010000300009000007";  //设备号ID
uint8_t DeviceID_temp[20] = {0};  //设备号ID
uint8_t serialdat_1[20] = {0};//设备号
uint8_t serialdat_2[20] = {0};//设备号
uint8_t serialdat_3[20] = {0};//数据1
uint8_t serialdat_4[20] = {0};//数据2
uint8_t serialdat_5[20] = {0};//数据3
uint8_t serialdat_6[20] = {0};//数据3
uint8_t testda[4] = {0x01,0x02,0,0};
uint16_t data_timer;//运动时间
uint8_t senddatflag = 0;
uint8_t Freq_Heart = 0;//心率
uint8_t stopflag = 0;
QueueHandle_t Key_Queue;   		//°′?ü?μ???￠?óáD??±ú
QueueHandle_t Message_Queue;	//D??￠?óáD??±ú
//线程句柄
TaskHandle_t task1_Handler;
TaskHandle_t task2_Handler;
TaskHandle_t task3_Handler;
void Task1(void *argument);
void Task2(void  *argument);
void Task3(void  *argument);

void SensorReData(void);
void SendDateToAndroid(int fr/*速度*/, int bmp/*心率*/, int dist/*里程*/,int tim/*时间*/,int tur/*热量*/);
int map(int x, int in_min, int in_max, int out_min, int out_max);
void Datasorting(void);//数据整合
SemaphoreHandle_t  xSemaphore = NULL;
void delay(u32 i)
{
	while(i--);
}

int main()
{
	delay_init();	    				//滴答定时器初始化 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //中断优先级分组 分2组
	//定时器初始化
	TIM3_Init(1000,71-1); //定时器中断初始化,      T = 1ms    F = 1kHz
	TIM4_CH1PWM_Init(500,71);   //TIM_CH1 PWM初始化
	//TIM1_Counter_Init(0xfff,3);
	//Timer1_Init();
	//串口初始化
	USART1_Init(57600);//蓝牙接口
	USART2_Init(9600);//备用串口
	USART3_Init(115200);//TFT屏
	//语音播放芯片初始化
	sound_Init();
	
	//传感器引脚初始化
	SENSOR_Init();//干簧管初始化
	SENSOR2_Init();//心率传感器初始化
	SensorIT_PB15_Init();//外部中断初始化
	//SensorIT_PA12_Init();
	SensorIT_PA0_Init();
	JDQ_Init();
	xSemaphore = xSemaphoreCreateBinary();
	JDQ = 0;
	//MotoInit();
	//ADC1_Init();
	//多线程初始化
	xTaskCreate(Task1,"task1",128,NULL, 0, &task1_Handler);
	xTaskCreate(Task2,"task2",128,NULL, 1, &task2_Handler);
	xTaskCreate(Task3,"task3",128,NULL, 2, &task3_Handler);
	vTaskStartScheduler();
	IWDG_Init(4,625);    //开门狗初始化	
	
	//xEventGroupCreate();
	//receiver();
	//Adc_Init();
	while(1)
	{
	
	}
}


void Task1(void *argument)   //adc检测线程
{
	int i = 0; uint16_t sys_stopTim = 0;
	//***********************ADC_DMA初始化必须放在此处**********************
		Init_All_Periph();
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
	//*********************************************************************
	for (;;)
	{
		
	   filter();
	   for(i=0;i<3;i++)
	   {
			value[i]= GetVolt(After_filter[i]);
	   }
	  // printf("AD7 value = %d mV  \r\n",value[0]);
	   //测试打印代码
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
void Task2(void *argument)  //串口线程
{
	uint8_t adress, i;

	//Message_Queue=xQueueCreate(2,50); //′′?¨???￠Message_Queue,?óáD??3¤?èê?′??ú?óê??o3???3¤?è
	for (;;)
	{
		
		//SendByte2(0xab);
		//PBout(13) = 1;
		//Modebus数据处理
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
					adress = Usart3Data[4]; //地址
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
void Task3(void *argument)  //语音播放和心率传感器线程
{	
	BaseType_t xResult;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300);
	for (;;)
	{   u8 len = 0;

		//TIM_SetCompare1(TIM4,Power);
		//SendByte2(0xac);
		//SensorReData();
	SensorReHeart();//心率传感器
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
	//语音播放私服部分=============================
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



void SendDateToAndroid(int fr/*速度*/, int bmp/*心率*/, int dist/*里程*/,int tim/*时间*/,int tur/*热量*/)
{
	uint8_t flg = 0;
	uint8_t h = 0,fen = 0, miao = 0;
	uint16_t destance = dist * K_PERIMETER; //周长*转数
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
	SendBuffer[3] = 0x82; //写指令
	SendBuffer[4] = 0; 
	SendBuffer[5] = 0x02;

	SendBuffer[6] = fr >> 8;  //表针   变量地址:0x0002
	SendBuffer[7] = fr & 0x00ff;

	SendBuffer[8] = bmp >> 8; //心率   变量地址:0x0003
	SendBuffer[9] = bmp & 0x00ff;

	//时，分，秒 计算
	h = tim / 3600;
	fen = (tim % 3600) / 60;
	miao = (tim % 3600) % 60;

	SendBuffer[10] = 0; //时  0x0004-0x0006
	SendBuffer[11] = h;

	SendBuffer[12] = 0; //fen
	SendBuffer[13] = fen;

	SendBuffer[14] = 0; //miao
	SendBuffer[15] = miao;

	SendBuffer[16] = dist >> 8; //里程   0x0007
	SendBuffer[17] = dist & 0x00ff;

	SendBuffer[18] = tur >> 8; //cal         0x0008
	SendBuffer[19] = tur & 0x00ff;

	SendBuffer[20] = 0; //速度值   0x0009
	SendBuffer[21] = flg;

	SendBuffer[22] = 0; //动画状态   0x000A
	SendBuffer[23] = flg;
	//uint16_t Chick = CRC16(SendBuffer, 9);
	SendBuffer[24] = 0;    //电量   0x000B
	SendBuffer[25] = map(value[0],0.0,3200.0,0.0,5.0);/*value[0]*/

	//Serial.write(SendBuffer, 26);//发送数据
	Uart3SendData(SendBuffer,26);
}
//范围映射函数-----用于把电池电量等效到1-10之间
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
void Datasorting()// 数据整合
{
	uint8_t i = 0;

	//第3帧数据
	serialdat_3[0] = 0xaa;
	serialdat_3[1] = 0x55;
	serialdat_3[2] = 0x03;
	serialdat_3[3] = data_timer >>8 ;
	serialdat_3[4] = data_timer & 0x00ff;
	serialdat_3[5] = distance >> 8;
    serialdat_3[6] = distance & 0x00ff;
	serialdat_3[7] = tmp_cal>>8;
	serialdat_3[8] = tmp_cal&0x00fff;//转速无
	serialdat_3[9] = Freq_Heart;
	serialdat_3[10] = 10;
	serialdat_3[11] = Freq >> 8;//坡度
	serialdat_3[12] = Freq & 0x00ff;//档位
	serialdat_3[13] = 13;//功率高位
	serialdat_3[14] = 14;//功率低位
	serialdat_3[15] = 15;//力矩高
	serialdat_3[16] = 16;//力矩低位
	serialdat_3[17] = 17;//校验位
	serialdat_3[18] = 18;//校验位
	serialdat_3[19] = 0xff;//校验位
	QF_CRC(serialdat_3,20);
	
		//第4帧数据
	serialdat_4[0] = 0xaa;
	serialdat_4[1] = 0x55;
	serialdat_4[2] = 0x04;
	serialdat_4[3] = 1;//模式
	serialdat_4[4] = 2;//负荷高
	serialdat_4[5] = 3;//负荷低
    serialdat_4[6] = 4;//频率
	serialdat_4[7] = hotdistance >> 8;
	serialdat_4[8] = hotdistance & 0x00ff;//次数
	serialdat_4[9] = 7;
	serialdat_4[10] = 8;//最大肌力高
	serialdat_4[11] = 9;//最大肌力低
	serialdat_4[12] = 10;
	serialdat_4[13] = ((190-Freq_Heart) *(65/100)+ 73)>>8;//最大摄氧量高
	serialdat_4[14] = ((190-Freq_Heart) *(65/100)+ 73) & 0x00ff;//最大摄氧量低位
	serialdat_4[15] = 0;//空
	serialdat_4[16] = 0;//空
	serialdat_4[17] = 0;//校验位
	serialdat_4[18] = 0;//校验位
	QF_CRC(serialdat_4,20);
	//第5帧数据
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

	//设备信息aa 55 01 30 30 30 4a 53 2d 35 30 32 38 00 00 00 00 00 00 f6

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

	//设备信息aa 55 01 30 30 30 4a 53 2d 35 30 32 38 00 00 00 00 00 00 f6

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