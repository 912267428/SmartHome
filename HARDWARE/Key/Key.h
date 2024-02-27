#ifndef _Key_H
#define _Key_H

#include "system.h"

/*  LEDʱ�Ӷ˿ڡ����Ŷ��� */
#define Key_PORT 			GPIOB

#define Key0_PIN 			GPIO_Pin_10
#define Key1_PIN 			GPIO_Pin_11

#define Key_PORT_RCC		RCC_APB2Periph_GPIOB

//ʹ��λ��������
#define KEY0 	PBin(10)
#define KEY1 	PBin(11)


//�����������ֵ
#define KEY_NO			0
#define KEY0_PRESS		1
#define KEY1_PRESS		2


void Key_Init(void);
u8 Key_GetNum(u8 mode);


#endif
