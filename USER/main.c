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

extern u8 alarmFlag = 0; //是否报警的标志
extern u8 alarm_is_busy = 0; //报警是否被手动操作

u8 humiH, humiL, tempH, tempL;
float light;

int main(void)
{	
	delay_init();	    //延时函数初始化
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	LED_Init();		  	//初始化与LED连接的硬件接口
	Buzzer_Init();
	KEY_Init();
	EXTIX_Init();
	
	TIM2_Int_Init(4999, 7199);
	OLED_Init();
	DHT11_Init();
	BH1750_Init();
	
	OLED_Clear();
	OLED_ShowString(1,1,"Smart Home");
	OLED_ShowString(2,1,"humi:  .");
	OLED_ShowString(3,1,"temp:  .");
	OLED_ShowString(4,1,"Light:      lx");
	
	
	while(1)
	{	
		//获取温湿度
		DHT11_Read_Data(&humiH, &humiL, &tempH, &tempL);
		//获取光强
		if (!i2c_CheckDevice(BH1750_Addr))
		{
			light = LIght_Intensity();
		}
		
		//判断是否报警
		if(light > 3000 || humiH > 80 || tempH > 32)
			alarmFlag = 1;
		else
			alarmFlag = 0;
		
		//串口与屏幕显示
		printf("humi:%d.%d, temp:%d.%d\r\n", humiH, humiL, tempH, tempL);
		OLED_ShowNum(2,6,humiH,2);
		OLED_ShowNum(2,9,humiL,2);
		OLED_ShowNum(3,6,tempH,2);
		OLED_ShowNum(3,9,tempL,2);
		
		OLED_ShowNum(4,7,light,5);
		
		//delay_ms(200);
	}
 }

