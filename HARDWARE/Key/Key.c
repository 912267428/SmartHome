#include "Key.h"
#include "delay.h"

/*******************************************************************************
* 函 数 名         : Key_Init
* 函数功能		   : 无线射频433M按键初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(Key_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=Key0_PIN | Key1_PIN | Key2_PIN| Key3_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //设置下拉输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(Key_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
}

u8 Key_GetNum(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //连续按键按下
		key=1;
	if (key==1&&(KEY0==1 || KEY1==1 || KEY2==1 || KEY3==1))	//任意按键按下
	{
		delay_ms(10);
		key = 0;
		if (KEY0 == 1)
			return KEY0_PRESS;
		else if(KEY1 == 1)
			return KEY1_PRESS;
		else if(KEY2 == 1)
			return KEY2_PRESS;
		else if(KEY3 == 1)
			return KEY3_PRESS;
	}
	else if (!(KEY0==1||KEY1==1||KEY2==1||KEY3==1)) //无按键按下
		key = 1;
	return KEY_NO;
}




