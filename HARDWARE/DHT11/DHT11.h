#ifndef __DHT11_H
#define __DHT11_H

#define DHT11_RCC RCC_APB2Periph_GPIOA 	//引脚RCC
#define DHT11_PORT GPIOA 				//定义DHT11的端口
#define DHT11_IO GPIO_Pin_15 			//定义DHT11的IO


void DHT11_IO_OUT (void);	//设置IO口为输出模式
void DHT11_IO_IN (void);	//设置IO口为输入模式
void DHT11_RST (void);		//复位DHT11
u8 DHT11_Check(void);		//检查DHT11是否正常
u8 DHT11_Init (void);		//初始化DHT11
u8 DHT11_Read_Bit(void);	//读取一位数据
u8 DHT11_Read_Byte(void);	//读取一个字节
u8 DHT11_Read_Data(u8 *humiH, u8 *humiL, u8 *tempH, u8 *tempL);	//读取DHT11的数据

#endif
