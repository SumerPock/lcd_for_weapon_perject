/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
	* 调试过程中发现的问题1：之前使用uartfifo调试速度慢的串口，比如200ms以上的速度接收发送时，未发现丢数据
	  但当我将速度提升至10ms这个级别，以115200bps 传输25Byte的数据时出错，首先115200
		的波特率传输速度极限在每秒11.25kByte/s , 而我只达到了2.5KByte/s就出现了很严重的
		丢包，事实证明FIFO只能缓解数据丢包现象，当我改成DMA传输是就没发现有丢包现象了
		
		问题2：我将串口数据发送DMA方式 放入定时器中发送，发现数据无法解析，接入逻辑分析仪后发现有时候
		串口数据会与发送的不一致
  ******************************************************************************
  */


#include "main.h"
#include "bsp.h"
#include "pic.h"
#include "SEGGER_RTT.h"

void SystemClock_Config(void);

CRC_HandleTypeDef hcrc;
IWDG_HandleTypeDef hiwdg = {0};


/*****************************任务*************************************/
void AppTaskStart(void *argument);		/*	启动任务	*/
osThreadId_t ThreadIdStart = NULL;			/* 启动任务线程ID */
/*启动任务*/
const osThreadAttr_t ThreadStart_Attr = 
{
	.name = "osRtxStartThread",			/* 线程名字  */
	.attr_bits = osThreadDetached,  /* 线程类型  */
	.priority = osPriorityHigh7,		/* 线程优先级 */
	.stack_size = 2048,							/* 堆栈的大小 */
};


void AppTaskLcdShow(void *argument);	/*	液晶屏幕显示任务	*/
osThreadId_t ThreadIdLcdShow = NULL;		/* 液晶屏幕显示任务线程ID */
/*液晶屏幕显示任务*/
const osThreadAttr_t ThreadLcdShow_Attr = 
{
	.name = "osRtxLcdShowThread",		/* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh6,		/* 线程优先级 */
	.stack_size = 2048,							/* 堆栈的大小 */
};


void AppTaskUart_1(void *argument);		/*	UART1任务	*/
osThreadId_t ThreadIdUart_1 = NULL;			/*UART1任务线程ID*/
/*UART1任务*/
const osThreadAttr_t ThreadUart_1_Attr = 
{
	.name = "osRtxUart1Thread",		  /* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh5,		/* 线程优先级 */
	.stack_size = 2048,							/* 堆栈的大小 */
};

void AppTaskUart_2(void *argument);		/*	UART2任务	*/
osThreadId_t ThreadIdUart_2 = NULL;			/*UART2任务线程ID*/
/*UART2任务*/
const osThreadAttr_t ThreadUart_2_Attr = 
{
	.name = "osRtxUart2Thread",		  /* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh4,		/* 线程优先级 */
	.stack_size = 2048,							/* 堆栈的大小 */
};


void AppTaskUart_4(void *argument);		/*	UART4任务	*/
osThreadId_t ThreadIdUart_4 = NULL;			/*UART4任务线程ID*/
/*UART4任务*/
const osThreadAttr_t ThreadUart_4_Attr = 
{
	.name = "osRtxUart4Thread",			/* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh3,		/* 线程优先级 */
	.stack_size = 2048,							/* 堆栈的大小 */
};

void AppTaskUart_6(void *argument);		/*	UART6任务	*/
osThreadId_t ThreadIdUart_6 = NULL;			/*UART6任务线程ID*/
/*UART6任务*/
const osThreadAttr_t ThreadUart_6_Attr = 
{
	.name = "osRtxUart6Thread",		  /* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh2,		/* 线程优先级 */
	.stack_size = 2048,							/* 堆栈的大小 */
};


void AppTaskRot_enc(void *argument);	/*	旋转编码器任务 Rotary encoder	*/
osThreadId_t ThreadIdRot_enc = NULL;		/*旋转编码器任务线程ID*/
/*旋转编码器任务*/
const osThreadAttr_t ThreadRot_enc_Attr = 
{
	.name = "osRtxRot_encThread",		/* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh6,		/* 线程优先级 */
//	.stack_size = 2048,							/* 堆栈的大小 */
	.stack_size = 512,							/* 堆栈的大小 */
};

void AppTaskGETKEYDATA(void *argument);		/*	按键任务	*/
osThreadId_t ThreadIdGetKey = NULL;				/* 按键任务线程ID */
const osThreadAttr_t ThreadGetKey = 			/*获取按键任务*/
{
	.name = "osRtxGetKeyThread",		/* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh6,		/* 线程优先级 */
	.stack_size = 1024,							/* 堆栈的大小 */
};


void AppTaskA_menu(void *argument);		/*	一级菜单选择任务	*/
osThreadId_t ThreadIdA_menu = NULL;		/* 一级菜单任务线程ID */
const osThreadAttr_t ThreadA_menu = 	/*一级菜单任务*/
{
	.name = "osRtxA_menuThread",		/* 线程名字   */
	.attr_bits = osThreadDetached,  /* 线程类型   */
	.priority = osPriorityHigh5,		/* 线程优先级 */
	.stack_size = 1024,							/* 堆栈的大小 */
};


/*****************************软件定时器*************************************/
/* 软件定时器 */
void timer_Periodic_App(void *argument);  
/* 保存定时器ID */
osTimerId_t  timerID_Periodic = NULL;   
/* 软件定时器1 属性  1ms*/
const osTimerAttr_t timer_Periodic_Attr = 
{
	.name = "periodic timer",        /* 定时器名字 */
};

/*****************************消息队列*************************************/
/*消息队列ID 传输电池信息*/
osMessageQueueId_t msgQueue_ButteryData;
/*建立消息队列 传递电池信息*/
const osMessageQueueAttr_t msgQueue_ButteryData_Attr =
{
	.name = "Message_Queue_ButteryData",
};

/*消息队列ID 传输LM75B温度信息*/
osMessageQueueId_t msgQueue_LM75BData;
/*建立消息队列 传递LM75B温度信息*/
const osMessageQueueAttr_t msgQueue_LM75BData_Attr =
{
	.name = "Message_Queue_LM75BData",
};


/*消息队列ID 传输旋转编码器的信息*/
osMessageQueueId_t msgQueue_Rotary_encoderData;

const osMessageQueueAttr_t msgQueue_Rotary_encoder_Attr =
{
	.name = "Message_Queue_Rotary_encoderData",
};


/*消息队列ID 传输按键键值*/
osMessageQueueId_t msgQueue_KeyData;

const osMessageQueueAttr_t msgQueue_KeyData_Attr =
{
	.name = "Message_Queue_KeyData",
};

/*消息队列ID 传输LM1266_1*/
osMessageQueueId_t msgQueue_Lm1266_Num1Data;
const osMessageQueueAttr_t msgQueue_Lm1266_Num1_Attr =
{
	.name = "Message_Queue_Lm1266_Num1",
};

/*消息队列ID 传输LM1266_2*/
osMessageQueueId_t msgQueue_Lm1266_Num2Data;
const osMessageQueueAttr_t msgQueue_Lm1266_Num2_Attr =
{
	.name = "Message_Queue_Lm1266_Num2",
};


/*****************************事件标志组*************************************/
/* 事件标志组属性 */
const osEventFlagsAttr_t event_DYTDataF_Attr = 
{
	.name = "event_DYTDataF",   	/* 事件标志组的名字 */
																/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_DYTDataF_ID = NULL; 


/*串口1 事件标志组属性 */
const osEventFlagsAttr_t event_USART1_Attr = 
{
	.name = "event_USART1",   	/* 事件标志组的名字 */
															/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_USART1_ID = NULL; 


/*串口2 事件标志组属性 */
const osEventFlagsAttr_t event_USART2_Attr = 
{
	.name = "event_USART2",   	/* 事件标志组的名字 */
															/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_USART2_ID = NULL; 


/*串口4 事件标志组属性 */
const osEventFlagsAttr_t event_UART4_Attr = 
{
	.name = "event_UART4",   	/* 事件标志组的名字 */
														/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_UART4_ID = NULL; 


/*LM75B 事件标志组属性 */
const osEventFlagsAttr_t event_LM75B_Attr = 
{
	.name = "event_LM75B",   	/* 事件标志组的名字 */
														/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_LM75B_ID = NULL; 


/*电池信息 事件标志组属性 */
const osEventFlagsAttr_t event_Battery_Attr = 
{
	.name = "event_Battery",   	/* 事件标志组的名字 */
															/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_Battery_ID = NULL; 


/*旋转编码器 事件标志组属性 */
const osEventFlagsAttr_t event_Rotary_encoder_Attr = 
{
	.name = "event_Rotary_encoder",   	/* 事件标志组的名字 */
																			/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_Rotary_encoder_ID = NULL; 



/*UI进入选择 事件标志组属性 choose*/
const osEventFlagsAttr_t event_UIChoose_Attr = 
{
	.name = "event_UIChoose",   	/* 事件标志组的名字 */
																			/* 其他默认 */
};
/* 标志组ID */
osEventFlagsId_t event_UIChoose_ID = NULL; 


///*屏幕UI进入选择 事件标志组属性 choose*/
//const osEventFlagsAttr_t event_UIScreenChoose_Attr = 
//{
//	.name = "event_UIScreenChoose",   	/* 事件标志组的名字 */
//																			/* 其他默认 */
//};
///* 标志组ID */
//osEventFlagsId_t event_UIScreenChoose_ID = NULL; 


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();	
  /* Infinite loop */
	HAL_SuspendTick();			/* 内核开启前关闭HAL的时间基准 */
	osKernelInitialize();  	/* 初始化RTX5内核 */ 	
	/* 创建启动任务 */
	ThreadIdStart = osThreadNew(AppTaskStart, NULL, &ThreadStart_Attr); 
	osKernelStart();	 			/* 启动RTX5内核 */
}



/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务，这里用作BSP驱动包处理。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: osPriorityHigh4  
*********************************************************************************************************
*/
void AppTaskStart(void *argument)
{
	unsigned char crctest[6] = {0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05};	
	const uint16_t usFrequency = 5000;	/* 延迟周期 */
	uint32_t tick;
	HAL_ResumeTick(); 					/* 重新开启HAL库心跳 */
	tick = osKernelGetTickCount(); 		/* 获取当前时间 */
	
	/*UART4 - 0
	  UART1 - 1*/
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	
	bsp_InitSPIBus();			/* 配置SPI总线 */	
	LCD_Init();						/* SPI LCD初始化*/
	//MX_IWDG_Init(&hiwdg);	/* 独立看门狗 */
	ConfigLed();
//	ConfigSP330(U1 , RS232); 	/* U1 422模式(UART1) */
//	ConfigSP330(U26 , RS422); /* U26 422模式(UART4) */
//	ConfigSP330(U8 , RS232);	/* U8 232模式(UART6) */
	MX_I2C1_Init();						/* I2C */
	ConfigOUT();

	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);			/* 配置通道0，上行配置*/
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);	/* 配置通道0，下行配置*/	
	SEGGER_RTT_SetTerminal(1);
	SEGGER_RTT_printf(0, "SEGGER_RTT_GetKey \r\n");	
	
	//bsp_InitUart();		/* 初始化fifo串口 , 由于fifo模式发现丢数据严重，改用DMA模式*/
	bsp_InitKey();
	
	MX_DMA_Init();
	
	MX_USART1_UART_Init();
	MX_UART4_UART_Init();
	MX_USART2_UART_Init();
	MX_USART6_UART_Init();
	
	/**UART DMA config**/
	HAL_UART_Transmit(&huart1, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	/* 开启串口空闲中断 */
	#define UART_RX_MODE_DMA	/* 开启串口接收 大小缓冲区最大长度rxSize */	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&huart1, g_tBspUart1.pRxBuf, g_tBspUart1.rxSize);
	#else
	HAL_UART_Receive_IT(&huart1, g_tBspUart1.pRxBuf, g_tBspUart1.rxSize);
	#endif
	
	HAL_UART_Transmit(&huart4, (uint8_t *)"this is uart4 send data!\r\n", 24, 1000);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);	/* 开启串口空闲中断 */
	#define UART_RX_MODE_DMA											/* 开启串口接收 大小缓冲区最大长度rxSize */
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&huart4, g_tBspUart4.pRxBuf, g_tBspUart4.rxSize);
	#else
	HAL_UART_Receive_IT(&huart4, g_tBspUart4.pRxBuf, g_tBspUart4.rxSize);
	#endif
	
	
	HAL_UART_Transmit(&husart2, (uint8_t *)"this is uart2 send data!\r\n", 24, 1000);
	__HAL_UART_ENABLE_IT(&husart2, UART_IT_IDLE);	/* 开启串口空闲中断 */
	#define UART_RX_MODE_DMA											/* 开启串口接收 大小缓冲区最大长度rxSize */
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#else
	HAL_UART_Receive_IT(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#endif


	HAL_UART_Transmit(&husart6, (uint8_t *)"this is usart6 send data!\r\n", 24, 1000);
	__HAL_UART_ENABLE_IT(&husart6, UART_IT_IDLE);	/* 开启串口空闲中断 */
	#define UART_RX_MODE_DMA											/* 开启串口接收 大小缓冲区最大长度rxSize */
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart6, g_tBspUsart6.pRxBuf, g_tBspUsart6.rxSize);
	#else
	HAL_UART_Receive_IT(&husart6, g_tBspUsart6.pRxBuf, g_tBspUsart6.rxSize);
	#endif
	
	LCD_Clear(BLACK);
	DISPlay_Open();
	tick += usFrequency + 1000;                          
	osDelayUntil(tick);	
	LCD_Clear(BLACK);
	DISPlay_Title();

	ThreadIdLcdShow = osThreadNew(AppTaskLcdShow, NULL, &ThreadLcdShow_Attr);
	/*链接孙工小源板*/
	//ThreadIdUart_1 = osThreadNew(AppTaskUart_1, NULL, &ThreadUart_1_Attr);	
	/*链接上位机*/
	//ThreadIdUart_4 = osThreadNew(AppTaskUart_4, NULL, &ThreadUart_4_Attr);	
	/*链接库仑计*/
	ThreadIdUart_2  = osThreadNew(AppTaskUart_2, NULL, &ThreadUart_2_Attr);	
	


	/*多级菜单显示*/
//	ThreadIdA_menu = osThreadNew(AppTaskA_menu, NULL, &ThreadA_menu);
//	
//	/*旋转编码器*/
//	ThreadIdRot_enc = osThreadNew(AppTaskRot_enc, NULL, &ThreadRot_enc_Attr);
//	
//	ThreadIdGetKey = osThreadNew(AppTaskGETKEYDATA, NULL, &ThreadGetKey);

//	/* 创建定时器 */
//	timerID_Periodic = osTimerNew(timer_Periodic_App,   /* 定时器回调函数 */
//																osTimerPeriodic,      /* 连续运行 */
//																NULL,              	/* 不传递参数 */
//																&timer_Periodic_Attr  /* 定时器属性 */
//															 );
//	osTimerStart(timerID_Periodic , 20U);/* 用于给蜂鸣器提供节拍 */
	
	msgQueue_ButteryData =	osMessageQueueNew(MQ_SIZE , sizeof(struct BUTTERY) ,&msgQueue_ButteryData_Attr);
	msgQueue_LM75BData   =	osMessageQueueNew(MQ_SIZE , sizeof(struct LM75BTEMP) ,&msgQueue_LM75BData_Attr);	
	msgQueue_Rotary_encoderData   =	osMessageQueueNew(MQ_SIZE , sizeof(struct Rotary_encoder) ,&msgQueue_Rotary_encoder_Attr);		
	msgQueue_KeyData   =	osMessageQueueNew(MQ_SIZE , sizeof(struct KEYDATA) ,&msgQueue_KeyData_Attr);
	
	msgQueue_Lm1266_Num1Data   =	osMessageQueueNew(MQ_SIZE , sizeof(struct LM1266Data) ,&msgQueue_Lm1266_Num1_Attr);
	msgQueue_Lm1266_Num2Data   =	osMessageQueueNew(MQ_SIZE , sizeof(struct LM1266Data) ,&msgQueue_Lm1266_Num2_Attr);

	event_DYTDataF_ID = osEventFlagsNew(&event_DYTDataF_Attr);	
	event_USART1_ID   = osEventFlagsNew(&event_USART1_Attr);
	event_USART2_ID   = osEventFlagsNew(&event_USART2_Attr);	
	event_UART4_ID    = osEventFlagsNew(&event_UART4_Attr);
	event_LM75B_ID    = osEventFlagsNew(&event_LM75B_Attr);
	event_Battery_ID    = osEventFlagsNew(&event_Battery_Attr);		
	//旋转编码器
	event_Rotary_encoder_ID    = osEventFlagsNew(&event_Rotary_encoder_Attr);	
	/*多级UI选择*/
	event_UIChoose_ID    = osEventFlagsNew(&event_UIChoose_Attr);		


	while(1)
	{	
		printf("The RTX5 OS is Run\r\n");			
		/* 相对延迟 */
		tick += usFrequency;                          
		osDelayUntil(tick);
		printf("Start App going to exit.\r\n");

		/* Start线程退出 */
		osThreadExit();  /* Start线程属于osThreadDetached,退出后无法通过osThreadJoin加入调度 */
	}
}



/*
*********************************************************************************************************
*	函 数 名: AppTaskA_menu
*	功能说明: 多级菜单UI显示
*	形    参: 无
*	返 回 值: 无
* 优 先 级:  
* 其    他: 这个函数中的功能比较复杂，一开始我只是简单的使用if else ,但发现使用简单的if else实现多级菜单切换异常
						困难，所以只能引入状态机函数这个终极杀手解决此问题
*********************************************************************************************************
*/
void AppTaskA_menu(void *argument)
{
	const uint16_t usFrequency = 5; /* 相对延时 */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	
	unsigned int Flag_Rotary_encoder = 0; 							// 旋转编码器
	static unsigned char msg_Rotary_encoder_Number = 0;
	struct Rotary_encoder getrot_enc;
	static unsigned char getfun = 0;		


	getfun = UI_CilckFun(1);//上电默认给一个1
	struct KEYDATA Getkeydata;
	
	static unsigned char msg_GetKeyDATA_Number = 0;	    //按键	
	static unsigned char keyenter = 0;//entern 按键

	unsigned int Flag_UIChoose = 0; 
	unsigned char cmos[9] = {0x81 , 0x01 , 0x04 , 0x47 , 0x00 , 0x00 , 0x00 , 0x00 , 0xff};
	unsigned short zoom_level[30] = 
				{0 , 0x16A1 , 0x2063 , 0x2628 , 0x2A1D , 0x2D13 , 0x2F6D , 0x3161 , 0x330D , 0x3486 , 
		0x35D7 , 0x3709 , 0x3820 , 0x3920 , 0x3A0A , 0x3ADD , 0x3B9C , 0x3C46 , 0x3CDC , 0x3D60 , 
		0x3DD4 , 0x3E39 , 0x3E90 , 0x3EDC , 0x3F1E , 0x3F57 , 0x3F8A , 0x3FB6 , 0x3FDC , 0x4000 };
	//DISPlay_DrawCircle_Break();	
	while(1)
	{
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);			
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskGETKEYDATA
*	功能说明: 获取按键键值
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskGETKEYDATA(void *argument)
{
	const uint16_t usFrequency = 10; /* 相对延时500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	uint8_t ucKeyCode; 
	struct KEYDATA getkeydata;
	unsigned char oldkeydata = 0;
	while(1)
	{
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskUart_1
*	功能说明: 链接孙工小原板
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskUart_1(void *argument)
{
	const uint16_t usFrequency = 25; /* 相对延时500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */	
	while(1)
	{	
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);
	}	
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 链接库仑计,查询LM75B信息
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskUart_2(void *argument)
{
	const uint16_t usFrequency = 500;/* 相对延时500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	
	static unsigned int loop = 0;	
	static unsigned char buffuart2[12] = {0};
	unsigned int Flag = 0;
//	unsigned char arrnbuff_0[] = {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x03 , 0x85 , 0xDD};
//	BSP_UART_Transmit_DMA(&husart6, &arrnbuff_0[0], 8);
//	BSP_UART_Transmit_DMA(&husart2, &arrnbuff_0[0], 8);	
//	BSP_UART_Transmit_DMA(&husart6, &arrnbuff_0[0], 8);
//	BSP_UART_Transmit_DMA(&husart2, &arrnbuff_0[0], 8);	
	struct LM1266Data lm1266num1;
	struct LM1266Data lm1266num2;
	while(1)
	{		
		Flag = osEventFlagsGet(event_USART2_ID);
		if(loop > 6)
		{
			loop = 0;
		}
		else
		{
			unsigned char sensortemp[2] = {0};
			unsigned char gettemp[2] = {0};				
			unsigned char aarnBuff[] = {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x03 , 0x85 , 0xDD};
			struct LM75BTEMP GetLM75Btempdata;
			switch(loop)
			{
				case 0:		
					BSP_UART_Transmit_DMA(&husart6, &aarnBuff[0], 8);
					break;
								
				case 1:
					BSP_UART_Transmit_DMA(&husart2, &aarnBuff[0], 8);
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
					HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
					if(HAL_I2C_Mem_Read(&hi2c1 , 0X90 , 0x00, I2C_MEMADD_SIZE_8BIT, sensortemp, 2, 1000) != HAL_OK)
					{
						printf("the I2C find error\r\n");
					}
					else
					{
						if(LM75BTemp(sensortemp , sizeof(sensortemp) , gettemp , sizeof(gettemp)))
						{
							GetLM75Btempdata.lm75b_buff[0] = gettemp[0];
							GetLM75Btempdata.lm75b_buff[1] = gettemp[1];
							os_Status = osMessageQueuePut(msgQueue_LM75BData, /* 消息队列的名字 */
																						&GetLM75Btempdata, 	/* 放入消息队列的变量（消息） */
																						NULL,      					/* 放入消息的优先级 */
																						NULL      	 				/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
							);				
							osEventFlagsSet(event_LM75B_ID , 0x01U << 0);  /*启动事件标志组 event_Flags1的flag0置1*/								
						}
						else
						{
							printf("temp = -");
						}
					}
					break;
				
				default:
					break;
			}
			if(loop < 2){
				loop++;		
			}else{
				loop = 0;
			}
		}		
		
		/* USER CODE BEGIN 3 */
		if(g_tBspUsart6.rxFlag == 1)
		{
			g_tBspUsart6.rxFlag = 0;
			//memcpy(&g_usart6_rx_buf,&g_tBspUsart6.pRxBuf,g_tBspUsart6.rxLen);
			if(g_usart6_rx_buf[1] == 0x03 && g_usart6_rx_buf[2] == 0x0C)
			{
				unsigned int nVoltage = g_usart6_rx_buf[3] << 24 | g_usart6_rx_buf[4] << 16 | g_usart6_rx_buf[5] << 8 | g_usart6_rx_buf[6];
				unsigned int nCurrent = g_usart6_rx_buf[7] << 24 | g_usart6_rx_buf[8] << 16 | g_usart6_rx_buf[9] << 8 | g_usart6_rx_buf[10];
				unsigned int nPower   = g_usart6_rx_buf[11] << 24 | g_usart6_rx_buf[12] << 16 | g_usart6_rx_buf[13] << 8 | g_usart6_rx_buf[14];
				
				SEGGER_RTT_SetTerminal(0);
				SEGGER_RTT_printf(0, "nVoltage = %d , nCurrent = %d , nPower = %d \r\n",nVoltage ,nCurrent ,nPower);	
				
				lm1266num1.arrVloat[0] = g_usart6_rx_buf[3];
				lm1266num1.arrVloat[1] = g_usart6_rx_buf[4];
				lm1266num1.arrVloat[2] = g_usart6_rx_buf[5];
				lm1266num1.arrVloat[3] = g_usart6_rx_buf[6];
				
				lm1266num1.arrPoweramp[0] = g_usart6_rx_buf[11];
				lm1266num1.arrPoweramp[1] = g_usart6_rx_buf[12];
				lm1266num1.arrPoweramp[2] = g_usart6_rx_buf[13];
				lm1266num1.arrPoweramp[3] = g_usart6_rx_buf[14];
				
				lm1266num1.arrCurrent[0] = g_usart6_rx_buf[7];
				lm1266num1.arrCurrent[1] = g_usart6_rx_buf[8];
				lm1266num1.arrCurrent[2] = g_usart6_rx_buf[9];
				lm1266num1.arrCurrent[3] = g_usart6_rx_buf[10];
				
				os_Status = osMessageQueuePut(msgQueue_Lm1266_Num1Data, /* 消息队列的名字 */
														&lm1266num1, 				/* 放入消息队列的变量（消息） */
														NULL,      					/* 放入消息的优先级 */
														NULL);      	 			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
			}
		}

		if(g_tBspUsart2.rxFlag == 1)
		{
			g_tBspUsart2.rxFlag = 0;		
			//memcpy(&g_usart2_rx_buf,&g_tBspUsart2.pRxBuf,g_tBspUsart2.rxLen);
			if(g_usart2_rx_buf[1] == 0x03 && g_usart2_rx_buf[2] == 0x0C)
			{
				unsigned int nVoltage = g_usart2_rx_buf[3] << 24 | g_usart2_rx_buf[4] << 16 | g_usart2_rx_buf[5] << 8 | g_usart2_rx_buf[6];
				unsigned int nCurrent = g_usart2_rx_buf[7] << 24 | g_usart2_rx_buf[8] << 16 | g_usart2_rx_buf[9] << 8 | g_usart2_rx_buf[10];
				unsigned int nPower   = g_usart2_rx_buf[11] << 24 | g_usart2_rx_buf[12] << 16 | g_usart2_rx_buf[13] << 8 | g_usart2_rx_buf[14];
					
				SEGGER_RTT_SetTerminal(1);
				SEGGER_RTT_printf(0, "nVoltage = %d , nCurrent = %d , nPower = %d \r\n",nVoltage ,nCurrent ,nPower);
				
				lm1266num2.arrVloat[0] = g_usart2_rx_buf[3];
				lm1266num2.arrVloat[1] = g_usart2_rx_buf[4];
				lm1266num2.arrVloat[2] = g_usart2_rx_buf[5];
				lm1266num2.arrVloat[3] = g_usart2_rx_buf[6];
				
				lm1266num2.arrPoweramp[0] = g_usart2_rx_buf[11];
				lm1266num2.arrPoweramp[1] = g_usart2_rx_buf[12];
				lm1266num2.arrPoweramp[2] = g_usart2_rx_buf[13];
				lm1266num2.arrPoweramp[3] = g_usart2_rx_buf[14];
				
				lm1266num2.arrCurrent[0] = g_usart2_rx_buf[7];
				lm1266num2.arrCurrent[1] = g_usart2_rx_buf[8];
				lm1266num2.arrCurrent[2] = g_usart2_rx_buf[9];
				lm1266num2.arrCurrent[3] = g_usart2_rx_buf[10];					
				
				os_Status = osMessageQueuePut(msgQueue_Lm1266_Num2Data, /* 消息队列的名字 */
									&lm1266num2, 				/* 放入消息队列的变量（消息） */
									NULL,      					/* 放入消息的优先级 */
									NULL);      	 			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */

			}					
		}		
				
		tick += usFrequency;  											/* 相对延时 */                          
		osDelayUntil(tick);
	}	
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 链接上位机通讯
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskUart_4(void *argument)
{
	const uint16_t usFrequency = 5; /* 相对延时500ms */	
	uint32_t tick;
	uint32_t Flag = 0;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	while(1)
	{
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);
	}	
}



/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskUart_6(void *argument)
{
	const uint16_t usFrequency = 50; /* 相对延时500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	while(1)
	{	
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);
	}	
}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: lcd液晶显示驱动函数
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskLcdShow(void *argument)
{
	unsigned char ucKeyCode;					/* 按键代码 */
	const uint16_t usFrequency = 100; /* 相对延时500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	
	struct LM1266Data lm1266num1;
	struct LM1266Data lm1266num2;
	
	static unsigned char msg_Number = 0;	
	static unsigned char msg_LM75B_Number = 0;
	static unsigned char msgLm1266Number1 = 0;
	static unsigned char msgLm1266Number2 = 0;
	
	static unsigned char msg_Rotary_encoder_Number = 0;
	//static unsigned char msg_GetKeyDATA_Number = 0;
	
	struct BUTTERY buttery;
	struct Rotary_encoder getrot_enc;
	struct LM75BTEMP LCDGetLM75Btempdata;

		
	unsigned int Flag_LM75B = 0;	
	unsigned int Flag_Battery = 0;
	//unsigned int Flag_Rotary_encoder = 0;
	
	static unsigned char getfun = 0;	
	static unsigned char returnuc = 0;//记录一下是否返回
	
	while(1)
	{
		Flag_LM75B = osEventFlagsGet(event_LM75B_ID);
		msg_LM75B_Number = osMessageQueueGetCount(msgQueue_LM75BData);
		
		Flag_Battery = osEventFlagsGet(event_Battery_ID);
		msg_Number = osMessageQueueGetCount(msgQueue_ButteryData);
		
		msgLm1266Number1 = osMessageQueueGetCount(msgQueue_Lm1266_Num1Data);
		msgLm1266Number2 = osMessageQueueGetCount(msgQueue_Lm1266_Num2Data);
		
		/*Temp*/
		if(Flag_LM75B && msg_LM75B_Number > 0)
		{
			if(osEventFlagsClear(event_USART1_ID , 0x01U) == (osFlagsErrorUnknown|osFlagsErrorParameter|osFlagsErrorResource))
			{			
				printf("EventFlagClrer is flad \r\n");
			}	
			os_Status = osMessageQueueGet(msgQueue_LM75BData, /* 消息队列的名字 */
																		&LCDGetLM75Btempdata,   					/* 存放消息的内存 */
																		NULL,              		/* 取出消息的优先级 */
																		100      		/* 超时时间，在中断函数里调用osMessageQueueGet时，此参数必须是NULL */);
			DISPlay_Temperature(&LCDGetLM75Btempdata.lm75b_buff[0] , 2);
		}
		if(msgLm1266Number1 > 0)
		{
			os_Status = osMessageQueueGet(msgQueue_Lm1266_Num1Data, 				/* 消息队列的名字 */
																		&lm1266num1,   					/* 存放消息的内存 */
																		NULL,              		/* 取出消息的优先级 */
																		100      		/* 超时时间，在中断函数里调用osMessageQueueGet时，此参数必须是NULL */);				
			DISPlay_Voltage_1(&lm1266num1.arrVloat[0] , 4);
			DISPlay_Current_1(&lm1266num1.arrCurrent[0] , 4);
			DISPlay_Poweramp_1(&lm1266num1.arrPoweramp[0] , 4);
			
			
		}
		if(msgLm1266Number2 > 0)
		{
				os_Status = osMessageQueueGet(msgQueue_Lm1266_Num2Data, 			/* 消息队列的名字 */
																		&lm1266num2,   					/* 存放消息的内存 */
																		NULL,              		/* 取出消息的优先级 */
																		100      		/* 超时时间，在中断函数里调用osMessageQueueGet时，此参数必须是NULL */);			
			DISPlay_Voltage_2(&lm1266num2.arrVloat[0] , 4);
			DISPlay_Current_2(&lm1266num2.arrCurrent[0] , 4);
			DISPlay_Poweramp_2(&lm1266num2.arrPoweramp[0] , 4);
		}
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);
	}
}



/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 讨厌的很，旋转编码器明明是给CPLD用的，非要在ARM上加入，所以不得不加入这个进程
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskRot_enc(void *argument)
{
	osStatus_t os_Status; 
	unsigned char ucKeyCode;		/* 按键代码 */
	unsigned int tick;
	const uint16_t usFrequency = 2; /* 相对延时500ms */	
	tick = osKernelGetTickCount();  /* 获取当前时间 */
	static int a = 0 , b = 0 , c = 0 , d = 0;
	//static char loop = 0;
	static char loop = 1;
	static char oldloop = 1;
	struct Rotary_encoder rot_enco_data;
	while(1)
	{
		tick += usFrequency;  /* 相对延时 */                          
		osDelayUntil(tick);
	}
}


/*
*********************************************************************************************************
*	函 数 名: timer_Periodic_App
*	功能说明: 软件定时器。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: osPriorityHigh4  
*********************************************************************************************************
*/
void timer_Periodic_App(void *argument)
{
	unsigned int tick = 0;	
	unsigned int longtime = 0;
	//tick = osKernelGetTickCount();	
	//longtime = bsp_GetRunTime();	/* 获取系统当前的time ticks */
}
