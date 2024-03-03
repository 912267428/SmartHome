#include "system.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "usart.h"
#include "timer.h"
#include "exti.h"

#include "OLED.h"
#include "led.h"
#include "Buzzer.h"
#include "DHT11.h"
#include "Bh1750.h"
#include "key.h"

#include "esp8266.h"
#include "onenet.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define INIT_TASK_PRIO		4
//任务堆栈大小	
#define INIT_STK_SIZE 		256  
//任务句柄
TaskHandle_t INITTask_Handler;
//任务函数
void init_task(void *pvParameters);

//任务优先级
#define Send_TASK_PRIO		2
//任务堆栈大小	
#define Send_STK_SIZE 		256  
//任务句柄
TaskHandle_t SendTask_Handler;
//任务函数
void send_task(void *pvParameters);

//任务优先级
#define Recieve_TASK_PRIO		2
//任务堆栈大小	
#define Recieve_STK_SIZE 		256  
//任务句柄
TaskHandle_t RecieveTask_Handler;
//任务函数
void recieve_task(void *pvParameters);

//任务优先级
#define Read_TASK_PRIO	1
//任务堆栈大小	
#define Read_STK_SIZE 		256  
//任务句柄
TaskHandle_t Read_Task_Handler;
//任务函数
void read_task(void *pvParameters);

extern u8 alarmFlag = 0; //是否报警的标志
extern u8 alarm_is_free = 20; //报警是否被手动操作
u8 LED_Status = 0;

u8 humiH, humiL, tempH, tempL;
float light;

char PUB_BUF[256]; //上传数据buf
const char *topics[] = {"/smarthome_yk/sub"};
const char devPubtopic[] = "/smarthome_yk/pub";

void Hardware_Init(void);
void OLED_ShowFixed(void);

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组4
		
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务 任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
    //创建发送任务
    xTaskCreate((TaskFunction_t )send_task,     
                (const char*    )"send_task",   
                (uint16_t       )Send_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Send_TASK_PRIO,
                (TaskHandle_t*  )&SendTask_Handler);
	
	//创建初始化任务
    xTaskCreate((TaskFunction_t )init_task,     
                (const char*    )"init_task",   
                (uint16_t       )INIT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )INIT_TASK_PRIO,
                (TaskHandle_t*  )&INITTask_Handler);
				
				
	//创建接收按键任务
    xTaskCreate((TaskFunction_t )recieve_task,     
                (const char*    )"recieve_task",   
                (uint16_t       )Recieve_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Recieve_TASK_PRIO,
                (TaskHandle_t*  )&RecieveTask_Handler);
				
	//创建读取数据任务
    xTaskCreate((TaskFunction_t )read_task,     
                (const char*    )"read_task",   
                (uint16_t       )Read_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Read_TASK_PRIO,
                (TaskHandle_t*  )&Read_Task_Handler); 
					
    vTaskDelete(StartTask_Handler); //删除开始任务	
    taskEXIT_CRITICAL();            //退出临界区
				
} 


void send_task(void *pvParameters)
{
    while(1)
    {
		vTaskSuspend(RecieveTask_Handler);
		vTaskSuspend(Read_Task_Handler);
		
		LED_Status = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
		UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
		sprintf(PUB_BUF,"{\"Temp\":%d.%d, \"Hum\":%d.%d, \"Light\":%.2f, \"LED\":%d, \"Buzzer\":%d}"
							,tempH,tempL,humiH,humiL,light,LED_Status?0:1,alarmFlag);
		OneNet_Publish(devPubtopic, PUB_BUF);

		ESP8266_Clear();
		
		vTaskResume(RecieveTask_Handler);
		vTaskResume(Read_Task_Handler);
		vTaskDelay(1500);
    }
}

void recieve_task(void *pvParameters)
{
	unsigned char *dataPtr = NULL;
	
	while(1)
	{
		vTaskSuspend(SendTask_Handler);
		vTaskSuspend(Read_Task_Handler);
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		vTaskResume(SendTask_Handler);
		vTaskResume(Read_Task_Handler);
		vTaskDelay(800);
	}
}


void read_task(void *pvParameters)
{
	
	while(1)
	{
		vTaskSuspend(SendTask_Handler);
		vTaskSuspend(RecieveTask_Handler);
		
		//获取温湿度
		DHT11_Read_Data(&humiH, &humiL, &tempH, &tempL);
		//获取光强
		if (!i2c_CheckDevice(BH1750_Addr))
		{
			light = LIght_Intensity();
		}
		//判断是否报警
		if(alarm_is_free >= 20) //如果报警器空闲，则运行自动控制 初始值为10
		{

			if(light > 3000 || humiH > 80 || tempH > 32)
				alarmFlag = 1;
			else
				alarmFlag = 0;
		}
		if(alarm_is_free < 20) alarm_is_free++;
		
		OLED_ShowNum(2,6,humiH,2);
		OLED_ShowNum(2,9,humiL,2);
		OLED_ShowNum(3,6,tempH,2);
		OLED_ShowNum(3,9,tempL,2);
		OLED_ShowNum(4,7,light,5);
		
		vTaskResume(SendTask_Handler);
		vTaskResume(RecieveTask_Handler);
		vTaskDelay(1000);
	}
}

void init_task(void *pvParameters)
{
	vTaskSuspend(SendTask_Handler);
	vTaskSuspend(RecieveTask_Handler);
	vTaskSuspend(Read_Task_Handler);
	
	Hardware_Init();
	
	OLED_ShowString(1,1,"Connecting WIFI");
	ESP8266_Init();
	
	OneNet_DevLink();
//	while(OneNet_DevLink())			//接入OneNET
//		delay_xms(500);
	
	OLED_Clear();
	OLED_ShowString(1,1,"Complete");
	printf("连接mqtt服务器成功\r\n");
	OLED_ShowFixed();
	
	//提示接入成功
	Buzzer_ON();
	delay_xms(200);
	Buzzer_OFF();
	
	OneNet_Subscribe(topics, 1);
	
	vTaskResume(SendTask_Handler);
	vTaskResume(RecieveTask_Handler);
	vTaskResume(Read_Task_Handler);
	vTaskDelete(INITTask_Handler); //删除任务
}

void Hardware_Init(void)
{
	Usart1_Init(115200);	//调试串口
	Usart2_Init(115200);	//stm32 to esp8266通讯串口
	
	LED_Init();		  	//初始化与LED连接的硬件接口
	Buzzer_Init();
	KEY_Init();
	OLED_Init();
	DHT11_Init();
	BH1750_Init();
	
	EXTIX_Init();
	
	TIM2_Int_Init(4999, 7199);
	TIM3_Int_Init(2499, 7199);
	
	OLED_ShowString(1,1,"Starting Up");
	printf("Hardware init OK\r\n");
}

void OLED_ShowFixed(void)
{
	OLED_Clear();
	OLED_ShowString(1,1,"Smart Home");
	OLED_ShowString(2,1,"humi:  .");
	OLED_ShowString(3,1,"temp:  .");
	OLED_ShowString(4,1,"Light:      lx");
}
