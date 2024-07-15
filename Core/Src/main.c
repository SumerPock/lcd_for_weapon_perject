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
	* ���Թ����з��ֵ�����1��֮ǰʹ��uartfifo�����ٶ����Ĵ��ڣ�����200ms���ϵ��ٶȽ��շ���ʱ��δ���ֶ�����
	  �����ҽ��ٶ�������10ms���������115200bps ����25Byte������ʱ��������115200
		�Ĳ����ʴ����ٶȼ�����ÿ��11.25kByte/s , ����ֻ�ﵽ��2.5KByte/s�ͳ����˺����ص�
		��������ʵ֤��FIFOֻ�ܻ������ݶ������󣬵��Ҹĳ�DMA�����Ǿ�û�����ж���������
		
		����2���ҽ��������ݷ���DMA��ʽ ���붨ʱ���з��ͣ����������޷������������߼������Ǻ�����ʱ��
		�������ݻ��뷢�͵Ĳ�һ��
  ******************************************************************************
  */


#include "main.h"
#include "bsp.h"
#include "pic.h"
#include "SEGGER_RTT.h"

void SystemClock_Config(void);

CRC_HandleTypeDef hcrc;
IWDG_HandleTypeDef hiwdg = {0};


/*****************************����*************************************/
void AppTaskStart(void *argument);		/*	��������	*/
osThreadId_t ThreadIdStart = NULL;			/* ���������߳�ID */
/*��������*/
const osThreadAttr_t ThreadStart_Attr = 
{
	.name = "osRtxStartThread",			/* �߳�����  */
	.attr_bits = osThreadDetached,  /* �߳�����  */
	.priority = osPriorityHigh7,		/* �߳����ȼ� */
	.stack_size = 2048,							/* ��ջ�Ĵ�С */
};


void AppTaskLcdShow(void *argument);	/*	Һ����Ļ��ʾ����	*/
osThreadId_t ThreadIdLcdShow = NULL;		/* Һ����Ļ��ʾ�����߳�ID */
/*Һ����Ļ��ʾ����*/
const osThreadAttr_t ThreadLcdShow_Attr = 
{
	.name = "osRtxLcdShowThread",		/* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh6,		/* �߳����ȼ� */
	.stack_size = 2048,							/* ��ջ�Ĵ�С */
};


void AppTaskUart_1(void *argument);		/*	UART1����	*/
osThreadId_t ThreadIdUart_1 = NULL;			/*UART1�����߳�ID*/
/*UART1����*/
const osThreadAttr_t ThreadUart_1_Attr = 
{
	.name = "osRtxUart1Thread",		  /* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh5,		/* �߳����ȼ� */
	.stack_size = 2048,							/* ��ջ�Ĵ�С */
};

void AppTaskUart_2(void *argument);		/*	UART2����	*/
osThreadId_t ThreadIdUart_2 = NULL;			/*UART2�����߳�ID*/
/*UART2����*/
const osThreadAttr_t ThreadUart_2_Attr = 
{
	.name = "osRtxUart2Thread",		  /* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh4,		/* �߳����ȼ� */
	.stack_size = 2048,							/* ��ջ�Ĵ�С */
};


void AppTaskUart_4(void *argument);		/*	UART4����	*/
osThreadId_t ThreadIdUart_4 = NULL;			/*UART4�����߳�ID*/
/*UART4����*/
const osThreadAttr_t ThreadUart_4_Attr = 
{
	.name = "osRtxUart4Thread",			/* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh3,		/* �߳����ȼ� */
	.stack_size = 2048,							/* ��ջ�Ĵ�С */
};

void AppTaskUart_6(void *argument);		/*	UART6����	*/
osThreadId_t ThreadIdUart_6 = NULL;			/*UART6�����߳�ID*/
/*UART6����*/
const osThreadAttr_t ThreadUart_6_Attr = 
{
	.name = "osRtxUart6Thread",		  /* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh2,		/* �߳����ȼ� */
	.stack_size = 2048,							/* ��ջ�Ĵ�С */
};


void AppTaskRot_enc(void *argument);	/*	��ת���������� Rotary encoder	*/
osThreadId_t ThreadIdRot_enc = NULL;		/*��ת�����������߳�ID*/
/*��ת����������*/
const osThreadAttr_t ThreadRot_enc_Attr = 
{
	.name = "osRtxRot_encThread",		/* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh6,		/* �߳����ȼ� */
//	.stack_size = 2048,							/* ��ջ�Ĵ�С */
	.stack_size = 512,							/* ��ջ�Ĵ�С */
};

void AppTaskGETKEYDATA(void *argument);		/*	��������	*/
osThreadId_t ThreadIdGetKey = NULL;				/* ���������߳�ID */
const osThreadAttr_t ThreadGetKey = 			/*��ȡ��������*/
{
	.name = "osRtxGetKeyThread",		/* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh6,		/* �߳����ȼ� */
	.stack_size = 1024,							/* ��ջ�Ĵ�С */
};


void AppTaskA_menu(void *argument);		/*	һ���˵�ѡ������	*/
osThreadId_t ThreadIdA_menu = NULL;		/* һ���˵������߳�ID */
const osThreadAttr_t ThreadA_menu = 	/*һ���˵�����*/
{
	.name = "osRtxA_menuThread",		/* �߳�����   */
	.attr_bits = osThreadDetached,  /* �߳�����   */
	.priority = osPriorityHigh5,		/* �߳����ȼ� */
	.stack_size = 1024,							/* ��ջ�Ĵ�С */
};


/*****************************�����ʱ��*************************************/
/* �����ʱ�� */
void timer_Periodic_App(void *argument);  
/* ���涨ʱ��ID */
osTimerId_t  timerID_Periodic = NULL;   
/* �����ʱ��1 ����  1ms*/
const osTimerAttr_t timer_Periodic_Attr = 
{
	.name = "periodic timer",        /* ��ʱ������ */
};

/*****************************��Ϣ����*************************************/
/*��Ϣ����ID ��������Ϣ*/
osMessageQueueId_t msgQueue_ButteryData;
/*������Ϣ���� ���ݵ����Ϣ*/
const osMessageQueueAttr_t msgQueue_ButteryData_Attr =
{
	.name = "Message_Queue_ButteryData",
};

/*��Ϣ����ID ����LM75B�¶���Ϣ*/
osMessageQueueId_t msgQueue_LM75BData;
/*������Ϣ���� ����LM75B�¶���Ϣ*/
const osMessageQueueAttr_t msgQueue_LM75BData_Attr =
{
	.name = "Message_Queue_LM75BData",
};


/*��Ϣ����ID ������ת����������Ϣ*/
osMessageQueueId_t msgQueue_Rotary_encoderData;

const osMessageQueueAttr_t msgQueue_Rotary_encoder_Attr =
{
	.name = "Message_Queue_Rotary_encoderData",
};


/*��Ϣ����ID ���䰴����ֵ*/
osMessageQueueId_t msgQueue_KeyData;

const osMessageQueueAttr_t msgQueue_KeyData_Attr =
{
	.name = "Message_Queue_KeyData",
};

/*��Ϣ����ID ����LM1266_1*/
osMessageQueueId_t msgQueue_Lm1266_Num1Data;
const osMessageQueueAttr_t msgQueue_Lm1266_Num1_Attr =
{
	.name = "Message_Queue_Lm1266_Num1",
};

/*��Ϣ����ID ����LM1266_2*/
osMessageQueueId_t msgQueue_Lm1266_Num2Data;
const osMessageQueueAttr_t msgQueue_Lm1266_Num2_Attr =
{
	.name = "Message_Queue_Lm1266_Num2",
};


/*****************************�¼���־��*************************************/
/* �¼���־������ */
const osEventFlagsAttr_t event_DYTDataF_Attr = 
{
	.name = "event_DYTDataF",   	/* �¼���־������� */
																/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_DYTDataF_ID = NULL; 


/*����1 �¼���־������ */
const osEventFlagsAttr_t event_USART1_Attr = 
{
	.name = "event_USART1",   	/* �¼���־������� */
															/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_USART1_ID = NULL; 


/*����2 �¼���־������ */
const osEventFlagsAttr_t event_USART2_Attr = 
{
	.name = "event_USART2",   	/* �¼���־������� */
															/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_USART2_ID = NULL; 


/*����4 �¼���־������ */
const osEventFlagsAttr_t event_UART4_Attr = 
{
	.name = "event_UART4",   	/* �¼���־������� */
														/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_UART4_ID = NULL; 


/*LM75B �¼���־������ */
const osEventFlagsAttr_t event_LM75B_Attr = 
{
	.name = "event_LM75B",   	/* �¼���־������� */
														/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_LM75B_ID = NULL; 


/*�����Ϣ �¼���־������ */
const osEventFlagsAttr_t event_Battery_Attr = 
{
	.name = "event_Battery",   	/* �¼���־������� */
															/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_Battery_ID = NULL; 


/*��ת������ �¼���־������ */
const osEventFlagsAttr_t event_Rotary_encoder_Attr = 
{
	.name = "event_Rotary_encoder",   	/* �¼���־������� */
																			/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_Rotary_encoder_ID = NULL; 



/*UI����ѡ�� �¼���־������ choose*/
const osEventFlagsAttr_t event_UIChoose_Attr = 
{
	.name = "event_UIChoose",   	/* �¼���־������� */
																			/* ����Ĭ�� */
};
/* ��־��ID */
osEventFlagsId_t event_UIChoose_ID = NULL; 


///*��ĻUI����ѡ�� �¼���־������ choose*/
//const osEventFlagsAttr_t event_UIScreenChoose_Attr = 
//{
//	.name = "event_UIScreenChoose",   	/* �¼���־������� */
//																			/* ����Ĭ�� */
//};
///* ��־��ID */
//osEventFlagsId_t event_UIScreenChoose_ID = NULL; 


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
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
	HAL_SuspendTick();			/* �ں˿���ǰ�ر�HAL��ʱ���׼ */
	osKernelInitialize();  	/* ��ʼ��RTX5�ں� */ 	
	/* ������������ */
	ThreadIdStart = osThreadNew(AppTaskStart, NULL, &ThreadStart_Attr); 
	osKernelStart();	 			/* ����RTX5�ں� */
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����������������BSP����������
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: osPriorityHigh4  
*********************************************************************************************************
*/
void AppTaskStart(void *argument)
{
	unsigned char crctest[6] = {0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05};	
	const uint16_t usFrequency = 5000;	/* �ӳ����� */
	uint32_t tick;
	HAL_ResumeTick(); 					/* ���¿���HAL������ */
	tick = osKernelGetTickCount(); 		/* ��ȡ��ǰʱ�� */
	
	/*UART4 - 0
	  UART1 - 1*/
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	
	bsp_InitSPIBus();			/* ����SPI���� */	
	LCD_Init();						/* SPI LCD��ʼ��*/
	//MX_IWDG_Init(&hiwdg);	/* �������Ź� */
	ConfigLed();
//	ConfigSP330(U1 , RS232); 	/* U1 422ģʽ(UART1) */
//	ConfigSP330(U26 , RS422); /* U26 422ģʽ(UART4) */
//	ConfigSP330(U8 , RS232);	/* U8 232ģʽ(UART6) */
	MX_I2C1_Init();						/* I2C */
	ConfigOUT();

	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);			/* ����ͨ��0����������*/
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);	/* ����ͨ��0����������*/	
	SEGGER_RTT_SetTerminal(1);
	SEGGER_RTT_printf(0, "SEGGER_RTT_GetKey \r\n");	
	
	//bsp_InitUart();		/* ��ʼ��fifo���� , ����fifoģʽ���ֶ��������أ�����DMAģʽ*/
	bsp_InitKey();
	
	MX_DMA_Init();
	
	MX_USART1_UART_Init();
	MX_UART4_UART_Init();
	MX_USART2_UART_Init();
	MX_USART6_UART_Init();
	
	/**UART DMA config**/
	HAL_UART_Transmit(&huart1, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA	/* �������ڽ��� ��С��������󳤶�rxSize */	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&huart1, g_tBspUart1.pRxBuf, g_tBspUart1.rxSize);
	#else
	HAL_UART_Receive_IT(&huart1, g_tBspUart1.pRxBuf, g_tBspUart1.rxSize);
	#endif
	
	HAL_UART_Transmit(&huart4, (uint8_t *)"this is uart4 send data!\r\n", 24, 1000);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA											/* �������ڽ��� ��С��������󳤶�rxSize */
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&huart4, g_tBspUart4.pRxBuf, g_tBspUart4.rxSize);
	#else
	HAL_UART_Receive_IT(&huart4, g_tBspUart4.pRxBuf, g_tBspUart4.rxSize);
	#endif
	
	
	HAL_UART_Transmit(&husart2, (uint8_t *)"this is uart2 send data!\r\n", 24, 1000);
	__HAL_UART_ENABLE_IT(&husart2, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA											/* �������ڽ��� ��С��������󳤶�rxSize */
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#else
	HAL_UART_Receive_IT(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#endif


	HAL_UART_Transmit(&husart6, (uint8_t *)"this is usart6 send data!\r\n", 24, 1000);
	__HAL_UART_ENABLE_IT(&husart6, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA											/* �������ڽ��� ��С��������󳤶�rxSize */
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
	/*�����﹤СԴ��*/
	//ThreadIdUart_1 = osThreadNew(AppTaskUart_1, NULL, &ThreadUart_1_Attr);	
	/*������λ��*/
	//ThreadIdUart_4 = osThreadNew(AppTaskUart_4, NULL, &ThreadUart_4_Attr);	
	/*���ӿ��ؼ�*/
	ThreadIdUart_2  = osThreadNew(AppTaskUart_2, NULL, &ThreadUart_2_Attr);	
	


	/*�༶�˵���ʾ*/
//	ThreadIdA_menu = osThreadNew(AppTaskA_menu, NULL, &ThreadA_menu);
//	
//	/*��ת������*/
//	ThreadIdRot_enc = osThreadNew(AppTaskRot_enc, NULL, &ThreadRot_enc_Attr);
//	
//	ThreadIdGetKey = osThreadNew(AppTaskGETKEYDATA, NULL, &ThreadGetKey);

//	/* ������ʱ�� */
//	timerID_Periodic = osTimerNew(timer_Periodic_App,   /* ��ʱ���ص����� */
//																osTimerPeriodic,      /* �������� */
//																NULL,              	/* �����ݲ��� */
//																&timer_Periodic_Attr  /* ��ʱ������ */
//															 );
//	osTimerStart(timerID_Periodic , 20U);/* ���ڸ��������ṩ���� */
	
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
	//��ת������
	event_Rotary_encoder_ID    = osEventFlagsNew(&event_Rotary_encoder_Attr);	
	/*�༶UIѡ��*/
	event_UIChoose_ID    = osEventFlagsNew(&event_UIChoose_Attr);		


	while(1)
	{	
		printf("The RTX5 OS is Run\r\n");			
		/* ����ӳ� */
		tick += usFrequency;                          
		osDelayUntil(tick);
		printf("Start App going to exit.\r\n");

		/* Start�߳��˳� */
		osThreadExit();  /* Start�߳�����osThreadDetached,�˳����޷�ͨ��osThreadJoin������� */
	}
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskA_menu
*	����˵��: �༶�˵�UI��ʾ
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:  
* ��    ��: ��������еĹ��ܱȽϸ��ӣ�һ��ʼ��ֻ�Ǽ򵥵�ʹ��if else ,������ʹ�ü򵥵�if elseʵ�ֶ༶�˵��л��쳣
						���ѣ�����ֻ������״̬����������ռ�ɱ�ֽ��������
*********************************************************************************************************
*/
void AppTaskA_menu(void *argument)
{
	const uint16_t usFrequency = 5; /* �����ʱ */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	
	unsigned int Flag_Rotary_encoder = 0; 							// ��ת������
	static unsigned char msg_Rotary_encoder_Number = 0;
	struct Rotary_encoder getrot_enc;
	static unsigned char getfun = 0;		


	getfun = UI_CilckFun(1);//�ϵ�Ĭ�ϸ�һ��1
	struct KEYDATA Getkeydata;
	
	static unsigned char msg_GetKeyDATA_Number = 0;	    //����	
	static unsigned char keyenter = 0;//entern ����

	unsigned int Flag_UIChoose = 0; 
	unsigned char cmos[9] = {0x81 , 0x01 , 0x04 , 0x47 , 0x00 , 0x00 , 0x00 , 0x00 , 0xff};
	unsigned short zoom_level[30] = 
				{0 , 0x16A1 , 0x2063 , 0x2628 , 0x2A1D , 0x2D13 , 0x2F6D , 0x3161 , 0x330D , 0x3486 , 
		0x35D7 , 0x3709 , 0x3820 , 0x3920 , 0x3A0A , 0x3ADD , 0x3B9C , 0x3C46 , 0x3CDC , 0x3D60 , 
		0x3DD4 , 0x3E39 , 0x3E90 , 0x3EDC , 0x3F1E , 0x3F57 , 0x3F8A , 0x3FB6 , 0x3FDC , 0x4000 };
	//DISPlay_DrawCircle_Break();	
	while(1)
	{
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);			
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGETKEYDATA
*	����˵��: ��ȡ������ֵ
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskGETKEYDATA(void *argument)
{
	const uint16_t usFrequency = 10; /* �����ʱ500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	uint8_t ucKeyCode; 
	struct KEYDATA getkeydata;
	unsigned char oldkeydata = 0;
	while(1)
	{
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUart_1
*	����˵��: �����﹤Сԭ��
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskUart_1(void *argument)
{
	const uint16_t usFrequency = 25; /* �����ʱ500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */	
	while(1)
	{	
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ӿ��ؼ�,��ѯLM75B��Ϣ
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskUart_2(void *argument)
{
	const uint16_t usFrequency = 500;/* �����ʱ500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	
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
							os_Status = osMessageQueuePut(msgQueue_LM75BData, /* ��Ϣ���е����� */
																						&GetLM75Btempdata, 	/* ������Ϣ���еı�������Ϣ�� */
																						NULL,      					/* ������Ϣ�����ȼ� */
																						NULL      	 				/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
							);				
							osEventFlagsSet(event_LM75B_ID , 0x01U << 0);  /*�����¼���־�� event_Flags1��flag0��1*/								
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
				
				os_Status = osMessageQueuePut(msgQueue_Lm1266_Num1Data, /* ��Ϣ���е����� */
														&lm1266num1, 				/* ������Ϣ���еı�������Ϣ�� */
														NULL,      					/* ������Ϣ�����ȼ� */
														NULL);      	 			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
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
				
				os_Status = osMessageQueuePut(msgQueue_Lm1266_Num2Data, /* ��Ϣ���е����� */
									&lm1266num2, 				/* ������Ϣ���еı�������Ϣ�� */
									NULL,      					/* ������Ϣ�����ȼ� */
									NULL);      	 			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */

			}					
		}		
				
		tick += usFrequency;  											/* �����ʱ */                          
		osDelayUntil(tick);
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ������λ��ͨѶ
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskUart_4(void *argument)
{
	const uint16_t usFrequency = 5; /* �����ʱ500ms */	
	uint32_t tick;
	uint32_t Flag = 0;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	while(1)
	{
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);
	}	
}



/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskUart_6(void *argument)
{
	const uint16_t usFrequency = 50; /* �����ʱ500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	while(1)
	{	
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);
	}	
}


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: lcdҺ����ʾ��������
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskLcdShow(void *argument)
{
	unsigned char ucKeyCode;					/* �������� */
	const uint16_t usFrequency = 100; /* �����ʱ500ms */	
	uint32_t tick;
	osStatus_t os_Status; 
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	
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
	static unsigned char returnuc = 0;//��¼һ���Ƿ񷵻�
	
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
			os_Status = osMessageQueueGet(msgQueue_LM75BData, /* ��Ϣ���е����� */
																		&LCDGetLM75Btempdata,   					/* �����Ϣ���ڴ� */
																		NULL,              		/* ȡ����Ϣ�����ȼ� */
																		100      		/* ��ʱʱ�䣬���жϺ��������osMessageQueueGetʱ���˲���������NULL */);
			DISPlay_Temperature(&LCDGetLM75Btempdata.lm75b_buff[0] , 2);
		}
		if(msgLm1266Number1 > 0)
		{
			os_Status = osMessageQueueGet(msgQueue_Lm1266_Num1Data, 				/* ��Ϣ���е����� */
																		&lm1266num1,   					/* �����Ϣ���ڴ� */
																		NULL,              		/* ȡ����Ϣ�����ȼ� */
																		100      		/* ��ʱʱ�䣬���жϺ��������osMessageQueueGetʱ���˲���������NULL */);				
			DISPlay_Voltage_1(&lm1266num1.arrVloat[0] , 4);
			DISPlay_Current_1(&lm1266num1.arrCurrent[0] , 4);
			DISPlay_Poweramp_1(&lm1266num1.arrPoweramp[0] , 4);
			
			
		}
		if(msgLm1266Number2 > 0)
		{
				os_Status = osMessageQueueGet(msgQueue_Lm1266_Num2Data, 			/* ��Ϣ���е����� */
																		&lm1266num2,   					/* �����Ϣ���ڴ� */
																		NULL,              		/* ȡ����Ϣ�����ȼ� */
																		100      		/* ��ʱʱ�䣬���жϺ��������osMessageQueueGetʱ���˲���������NULL */);			
			DISPlay_Voltage_2(&lm1266num2.arrVloat[0] , 4);
			DISPlay_Current_2(&lm1266num2.arrCurrent[0] , 4);
			DISPlay_Poweramp_2(&lm1266num2.arrPoweramp[0] , 4);
		}
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);
	}
}



/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����ĺܣ���ת�����������Ǹ�CPLD�õģ���Ҫ��ARM�ϼ��룬���Բ��ò������������
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskRot_enc(void *argument)
{
	osStatus_t os_Status; 
	unsigned char ucKeyCode;		/* �������� */
	unsigned int tick;
	const uint16_t usFrequency = 2; /* �����ʱ500ms */	
	tick = osKernelGetTickCount();  /* ��ȡ��ǰʱ�� */
	static int a = 0 , b = 0 , c = 0 , d = 0;
	//static char loop = 0;
	static char loop = 1;
	static char oldloop = 1;
	struct Rotary_encoder rot_enco_data;
	while(1)
	{
		tick += usFrequency;  /* �����ʱ */                          
		osDelayUntil(tick);
	}
}


/*
*********************************************************************************************************
*	�� �� ��: timer_Periodic_App
*	����˵��: �����ʱ����
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: osPriorityHigh4  
*********************************************************************************************************
*/
void timer_Periodic_App(void *argument)
{
	unsigned int tick = 0;	
	unsigned int longtime = 0;
	//tick = osKernelGetTickCount();	
	//longtime = bsp_GetRunTime();	/* ��ȡϵͳ��ǰ��time ticks */
}
