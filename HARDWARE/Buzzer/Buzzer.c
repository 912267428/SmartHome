#include "stm32f10x.h"                  // Device header
#include "Buzzer.h"

/**
  * @brief 初始化蜂鸣器对应的IO口
  * @param 
  * @retval 
  */
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	Buzzer_OFF();
}

/**
  * @brief 打开蜂鸣器
  * @param 
  * @retval 
  */
void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

/**
  * @brief 关闭蜂鸣器
  * @param 
  * @retval 
  */
void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

void Buzzer_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)
	{
		Buzzer_OFF();
	}
	else{
		Buzzer_ON();
	}
}
