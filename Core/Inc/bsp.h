/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For STM32F407)
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��
*		V1.0    2018-07-29  Eric2013   ��ʽ����
*
*	Copyright (C), 2018-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#define STM32_V5    


/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_V5)
	#error "Please define the board model : STM32_V5"
#endif

/* ���� BSP �汾�� */
#define __STM32H7_BSP_VERSION		"1.1"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#define EXTI9_5_ISR_MOVE_OUT		/* bsp.h �ж�����У���ʾ�������Ƶ� stam32f4xx_it.c�� �����ظ����� */

#define ERROR_HANDLER()		Error_Handler(__FILE__, __LINE__);

#define  USE_RTX    1

/* Ĭ���ǹر�״̬ */
#define  Enable_EventRecorder  1

#if Enable_EventRecorder == 1
	#include "EventRecorder.h"
#endif

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* �������ȼ����� */
#define NVIC_PREEMPT_PRIORITY	4

#include "lcd_init.h"
#include "lcd.h"


//#include "GuiLite.h"
//#include "stdlib.h"	

#include "includes.h"

#include "bsp_uart.h"
#include "bsp_dma.h"
#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "bsp_dwt.h"
#include "bsp_key.h"
#include "bsp_uart_fifo.h"
#include "bsp_spi_bus.h"
#include "bsp_i2c.h"
#include "bsp_lcdui.h"


typedef struct
{
	unsigned char *pRxBuf;
	unsigned char rxFlag;
	unsigned char rxLen;
	const unsigned char rxSize;	/*const �����ڽṹ���������ʱ��ֵ */
}BspUart_t;

extern BspUart_t g_tBspUart1;
extern BspUart_t g_tBspUart4;
extern BspUart_t g_tBspUsart2;
extern BspUart_t g_tBspUsart6;


#define MQ_SIZE 16

struct BUTTERY
{
	unsigned char 	ButryStae;					//״̬
	unsigned short 	Voltage;						//��ѹ
	unsigned short 	Electric_current;		//����
	unsigned short 	Butryremaining;			//ʣ��ٷֱ�
	unsigned short 	Butrycapacity;			//����
	unsigned char 	add; 			
	unsigned char 	temperature;				//�¶�
};

struct LM1266Data
{
	unsigned char arrCurrent[4];
	unsigned char arrVloat[4];
	unsigned char arrPoweramp[4];
};


struct LM75BTEMP
{
	unsigned char lm75b_buff[2];
	unsigned char lm75b_temp;
};

struct Rotary_encoder
{
	unsigned char Rotary_encoder;
};
//extern struct Rotary_encoder;

struct KEYDATA
{
	unsigned char Key_Data;
};
//extern struct KEYDATA;

enum SP330MODE            /* Defines an enumeration type    */
{
	RS232 = 0,
	RS422 = 1,
	RS485 = 2
};

enum SP330NUM            /* Defines an enumeration type    */
{
	U1 = 0,
	U7 = 1,
	U8 = 2,
	U26 = 3
};


enum DYTState
{
	initialize = 0,
	capture = 1,
	not_caught = 2
};


enum DYTPlace
{
	center = 0, //����
	
	north = 1,	//��
	south = 2, //��
	west = 3, //��
	east = 4, //��
	
	north_by_west = 5, 		//��ƫ��
	north_by_east = 6, 		//��ƫ��
	south_by_west = 7, 	//��ƫ��
	south_by_east = 8 		//��ƫ��
};
void DISPlay_image_park(enum DYTState dyts , enum DYTPlace dytp);


//����һ���ṹ����������������
typedef struct
{
	int current;			//��ǰ״̬������
	int next; 				//����һ��
	int enter;				//ȷ��
	void (*current_operation)(unsigned char selected);	//��ǰ״̬Ӧ��ִ�еĲ���
	//void (*current_operation)(void);	
}Menu;





extern void ConfigSP330(enum SP330NUM sn , enum SP330MODE sm);

extern osMessageQueueId_t msgQueue_ButteryData;

extern osEventFlagsId_t event_DYTDataF_ID;

extern osEventFlagsId_t event_Battery_ID;

extern osEventFlagsId_t event_USART1_ID;

extern osEventFlagsId_t event_USART2_ID;

extern I2C_HandleTypeDef hi2c1;

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);
void Error_Handler(void);
void bsp_GetCpuID(uint32_t *_id);
void MX_IWDG_Init(IWDG_HandleTypeDef *idwg);
void MX_GPIO_Init(void);
void SystemClock_Config(void);
unsigned char DYTCommData(unsigned char* buff , unsigned char buffled);
unsigned char bsp_buttery_protocol(unsigned char *getbuff , unsigned char getbuffsize);
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
