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
	delay_init();	    //延时函数初始化
	uart_init(115200);
	 
	LED_Init();		  	//初始化与LED连接的硬件接口
	OLED_Init();
	
	while(1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4); //LED0输出低
		GPIO_SetBits(GPIOC,GPIO_Pin_13);//LED1输出高
		delay_ms(300);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);//LED0输出高
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);//LED1输出低
		delay_ms(300);
	}
 }

