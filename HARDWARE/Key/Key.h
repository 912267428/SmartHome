#ifndef _Key_H
#define _Key_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define Key_PORT 			GPIOB
#define Key0_PIN 			GPIO_Pin_10
#define Key1_PIN 			GPIO_Pin_11
#define Key2_PIN 			GPIO_Pin_1
#define Key3_PIN 			GPIO_Pin_0

#define Key_PORT_RCC		RCC_APB2Periph_GPIOB

//使用位操作定义
#define KEY0 	PBin(10)
#define KEY1 	PBin(11)
#define KEY2 	PBin(1)
#define KEY3 	PBin(0)

//定义各个按键值
#define KEY_NO			0
#define KEY0_PRESS		1
#define KEY1_PRESS		2
#define KEY2_PRESS		3
#define KEY3_PRESS		4

void Key_Init(void);
u8 Key_GetNum(u8 mode);


#endif
