#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

#include "led.h"
#include "OLED.h"
#include "Buzzer.h"
#include "Key.h"
#include "DHT11.h"
#include "Bh1750.h"

u8 alarmFlag = 0; //是否报警的标志

u8 KeyNum = KEY_NO;
u8 humiH, humiL, tempH, tempL;
float light;

int main(void)
{	
	delay_init();	    //延时函数初始化
	uart_init(115200);
	
	LED_Init();		  	//初始化与LED连接的硬件接口
	Buzzer_Init();
	TIM2_Int_Init(4999, 7199);
	OLED_Init();
	Key_Init();
	DHT11_Init();
	BH1750_Init();
	
	OLED_Clear();
	OLED_ShowString(1,1,"Smart Home");
	OLED_ShowString(2,1,"humi:  .");
	OLED_ShowString(3,1,"temp:  .");
	OLED_ShowString(4,1,"Light:      lx");
	
	
	while(1)
	{	
		KeyNum = Key_GetNum(0);
		if(KeyNum == KEY0_PRESS)
		{
			printf("key0 press\r\n");
		}else if(KeyNum == KEY1_PRESS)
		{
			printf("key1 press\r\n");
		}
		
		//获取温湿度
		DHT11_Read_Data(&humiH, &humiL, &tempH, &tempL);
		if(humiH > 80 || tempH > 30) alarmFlag = 1;
		
		//获取光强
		if (!i2c_CheckDevice(BH1750_Addr))
		{
			light = LIght_Intensity();
		}
		if(light > 10000) alarmFlag = 1;
		
		printf("humi:%d.%d, temp:%d.%d\r\n", humiH, humiL, tempH, tempL);
		OLED_ShowNum(2,6,humiH,2);
		OLED_ShowNum(2,9,humiL,2);
		OLED_ShowNum(3,6,tempH,2);
		OLED_ShowNum(3,9,tempL,2);
		
		OLED_ShowNum(4,7,light,5);
		
		delay_ms(200);
	}
 }

