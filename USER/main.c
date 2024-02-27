#include "delay.h"
#include "sys.h"
#include "usart.h"

#include "led.h"
#include "OLED.h"
#include "Buzzer.h"
#include "Key.h"
#include "DHT11.h"


int main(void)
{	
	u8 KeyNum = KEY_NO;
	u8 humiH, humiL, tempH, tempL;
	
	delay_init();	    //延时函数初始化
	uart_init(115200);
	
	LED_Init();		  	//初始化与LED连接的硬件接口
	OLED_Init();
	Key_Init();
	DHT11_Init();
	
	OLED_ShowString(1,1,"Smart Home");
	OLED_ShowString(2,1,"humi:  .");
	OLED_ShowString(3,1,"temp:  .");
	
	
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
		
		DHT11_Read_Data(&humiH, &humiL, &tempH, &tempL);
		printf("humi:%d.%d, temp:%d.%d\r\n", humiH, humiL, tempH, tempL);
		OLED_ShowNum(2,6,humiH,2);
		OLED_ShowNum(2,9,humiL,2);
		OLED_ShowNum(3,6,tempH,2);
		OLED_ShowNum(3,9,tempL,2);
			
		delay_ms(1000);
	}
 }

