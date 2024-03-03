#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY0 PBin(0)   	
#define KEY1 PBin(1)	 
 

//#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//读取按键0
//#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//读取按键1
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
