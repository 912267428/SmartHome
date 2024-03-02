#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"

#include "led.h"
#include "OLED.h"
#include "Buzzer.h"
#include "DHT11.h"
#include "Bh1750.h"
#include "key.h"

#include "esp8266.h"
#include "onenet.h"

extern u8 alarmFlag = 0; //是否报警的标志
extern u8 alarm_is_free = 10; //报警是否被手动操作

u8 humiH, humiL, tempH, tempL;
float light;

char PUB_BUF[256]; //上传数据buf
const char *topics[] = {"/smarthome_yk/sub"};
const char devPubtopic[] = "/smarthome_yk/pub";

void Hardware_Init(void);
void OLED_ShowFixed(void);

int main(void)
{	
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();
	OLED_ShowFixed();
	
	ESP8266_Init();
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	
	//提示接入成功
	Buzzer_ON();
	delay_ms(200);
	Buzzer_OFF();
	
	OneNet_Subscribe(topics, 1);
	
	while(1) 
	{	
		if(timeCount % 40 == 0){  //1000ms / 25 = 40
			//获取温湿度
			DHT11_Read_Data(&humiH, &humiL, &tempH, &tempL);
			//获取光强
			if (!i2c_CheckDevice(BH1750_Addr))
			{
				light = LIght_Intensity();
			}
			//判断是否报警
			if(alarm_is_free >= 10) //如果报警器空闲，则运行自动控制 初始值为10
			{

				if(light > 3000 || humiH > 80 || tempH > 32)
					alarmFlag = 1;
				else
					alarmFlag = 0;
			}
			if(alarm_is_free < 10) alarm_is_free++;
			//printf("alarm_is_free:%d, alarmFlag:%d\r\n",alarm_is_free,alarmFlag);
			//UsartPrintf(USART_DEBUG, "alarm_is_free:%d, alarmFlag:%d\r\n",alarm_is_free,alarmFlag);
			printf("当前温度:%d.%d  湿度:%d.%d\r\n",tempH,tempL,humiH,humiL);
			printf("当前光照强度:%.2f lx\r\n",light);
			//屏幕显示
			OLED_ShowNum(2,6,humiH,2);
			OLED_ShowNum(2,9,humiL,2);
			OLED_ShowNum(3,6,tempH,2);
			OLED_ShowNum(3,9,tempL,2);
			OLED_ShowNum(4,7,light,5);
		}
		
		if(++timeCount >= 200)	//发送间隔5s  5000ms / 25 = 200
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf(PUB_BUF,"{\"Temp\":%d.%d, \"Hum\":%d.%d, \"Light\":%.2f}",tempH,tempL,humiH,humiL,light);
			OneNet_Publish(devPubtopic, PUB_BUF);
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	}
 }

void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    //延时函数初始化
	
	Usart1_Init(115200);	//调试串口
	Usart2_Init(115200);	//stm32 to esp8266通讯串口
	
	LED_Init();		  	//初始化与LED连接的硬件接口
	Buzzer_Init();
	KEY_Init();
	OLED_Init();
	DHT11_Init();
	BH1750_Init();
	
	EXTIX_Init();
	
	TIM2_Int_Init(4999, 7199);
	TIM3_Int_Init(2499, 7199);
	
	OLED_ShowString(1,1,"Starting Up");
	delay_ms(300);
	printf("Hardware init OK\r\n");
	OLED_ShowString(1,1,"Completed");
	delay_ms(200);
}

void OLED_ShowFixed(void)
{
	OLED_Clear();
	OLED_ShowString(1,1,"Smart Home");
	OLED_ShowString(2,1,"humi:  .");
	OLED_ShowString(3,1,"temp:  .");
	OLED_ShowString(4,1,"Light:      lx");
}
