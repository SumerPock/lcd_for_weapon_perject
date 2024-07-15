/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32F407)
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2018-07-29  Eric2013   正式发布
*
*	Copyright (C), 2018-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#define STM32_V5    


/* 检查是否定义了开发板型号 */
#if !defined (STM32_V5)
	#error "Please define the board model : STM32_V5"
#endif

/* 定义 BSP 版本号 */
#define __STM32H7_BSP_VERSION		"1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#define EXTI9_5_ISR_MOVE_OUT		/* bsp.h 中定义此行，表示本函数移到 stam32f4xx_it.c。 避免重复定义 */

#define ERROR_HANDLER()		Error_Handler(__FILE__, __LINE__);

#define  USE_RTX    1

/* 默认是关闭状态 */
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

/* 定义优先级分组 */
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
	const unsigned char rxSize;	/*const 必须在结构体变量定义时赋值 */
}BspUart_t;

extern BspUart_t g_tBspUart1;
extern BspUart_t g_tBspUart4;
extern BspUart_t g_tBspUsart2;
extern BspUart_t g_tBspUsart6;


#define MQ_SIZE 16

struct BUTTERY
{
	unsigned char 	ButryStae;					//状态
	unsigned short 	Voltage;						//电压
	unsigned short 	Electric_current;		//电流
	unsigned short 	Butryremaining;			//剩余百分比
	unsigned short 	Butrycapacity;			//容量
	unsigned char 	add; 			
	unsigned char 	temperature;				//温度
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
	center = 0, //中心
	
	north = 1,	//北
	south = 2, //南
	west = 3, //西
	east = 4, //东
	
	north_by_west = 5, 		//北偏西
	north_by_east = 6, 		//北偏东
	south_by_west = 7, 	//南偏西
	south_by_east = 8 		//南偏东
};
void DISPlay_image_park(enum DYTState dyts , enum DYTPlace dytp);


//定义一个结构体包含你想操作内容
typedef struct
{
	int current;			//当前状态索引号
	int next; 				//向下一个
	int enter;				//确定
	void (*current_operation)(unsigned char selected);	//当前状态应该执行的操作
	//void (*current_operation)(void);	
}Menu;





extern void ConfigSP330(enum SP330NUM sn , enum SP330MODE sm);

extern osMessageQueueId_t msgQueue_ButteryData;

extern osEventFlagsId_t event_DYTDataF_ID;

extern osEventFlagsId_t event_Battery_ID;

extern osEventFlagsId_t event_USART1_ID;

extern osEventFlagsId_t event_USART2_ID;

extern I2C_HandleTypeDef hi2c1;

/* 提供给其他C文件调用的函数 */
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
