#ifndef __DHT11_H
#define __DHT11_H

#define DHT11_RCC RCC_APB2Periph_GPIOA 	//����RCC
#define DHT11_PORT GPIOA 				//����DHT11�Ķ˿�
#define DHT11_IO GPIO_Pin_15 			//����DHT11��IO


void DHT11_IO_OUT (void);	//����IO��Ϊ���ģʽ
void DHT11_IO_IN (void);	//����IO��Ϊ����ģʽ
void DHT11_RST (void);		//��λDHT11
u8 DHT11_Check(void);		//���DHT11�Ƿ�����
u8 DHT11_Init (void);		//��ʼ��DHT11
u8 DHT11_Read_Bit(void);	//��ȡһλ����
u8 DHT11_Read_Byte(void);	//��ȡһ���ֽ�
u8 DHT11_Read_Data(u8 *humiH, u8 *humiL, u8 *tempH, u8 *tempL);	//��ȡDHT11������

#endif
