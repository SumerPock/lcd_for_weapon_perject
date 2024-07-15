/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.h
*	版    本 : V1.3
*	说    明 : 头文件
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

/*
	在此定义若干个软件定时器全局变量
	注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT	4		/* 软件定时器的个数 （定时器ID范围 0 - 3) */

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
}TMR_MODE_E;

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile unsigned char Mode;		/* 计数器模式，1次性 */
	volatile unsigned char Flag;		/* 定时到达标志  */
	volatile unsigned int Count;	/* 计数器 */
	volatile unsigned int PreLoad;	/* 计数器预装值 */
}SOFT_TMR;

/* 提供给其他C文件调用的函数 */
void bsp_InitTimer(void);
void bsp_DelayMS(unsigned int n);
void bsp_DelayUS(unsigned int n);
void bsp_StartTimer(unsigned char _id, unsigned int _period);
void bsp_StartAutoTimer(unsigned char _id, unsigned int _period);
void bsp_StopTimer(unsigned char _id);
unsigned char bsp_CheckTimer(unsigned char _id);
unsigned int bsp_GetRunTime(void);
unsigned int bsp_CheckRunTime(unsigned int _LastTime);

void bsp_InitHardTimer(void);
void bsp_StartHardTimer(unsigned char _CC, unsigned int _uiTimeOut, void * _pCallBack);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
