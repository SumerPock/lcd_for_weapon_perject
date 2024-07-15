/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp.h"
#include "stm32f4xx_it.h"

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}



/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART6_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&husart6);
  HAL_UART_IRQHandler(&husart6);
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&huart1);
  HAL_UART_IRQHandler(&huart1);
}


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART2_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&husart2);
  HAL_UART_IRQHandler(&husart2);
}


/*
*********************************************************************************************************
*	函 数 名: UART4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void UART4_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&huart4);
  HAL_UART_IRQHandler(&huart4);
}


/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream2_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
}


/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
}


/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream2_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream7_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream6_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}


/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream5_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream6_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream7_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:  
stm32f4xx_it.c 调用
*********************************************************************************************************
*/
void BSP_UART_IDLE_Handler(UART_HandleTypeDef *huart)
{
	unsigned int rxCnt = 0;
	if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET)/* 开启了空闲中断位才进入判断 */
	{
		
		if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)/* 是空闲中断 */
		{	
			__HAL_UART_CLEAR_IDLEFLAG(huart);	/* 清除串口空闲中断 */
			unsigned char dmaBit = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
			if(dmaBit)
			{		
				rxCnt = __HAL_DMA_GET_COUNTER(huart->hdmarx);/* 从DMA寄存器中读取剩余空间寄存器值 */
			}
			else
			{
				rxCnt = huart->RxXferCount;
			}		
			HAL_UART_AbortReceive(huart);/* 关闭串口接收 */
			
			/* 串口1中断 */
			if(huart->Instance == USART1)
			{
				g_tBspUart1.rxLen  = g_tBspUart1.rxSize - rxCnt;
				g_tBspUart1.rxFlag = 1;
				
				osEventFlagsSet(event_USART1_ID , 0x01U << 0);  /*启动事件标志组 event_Flags1的flag0置1*/				
				/* 重新开启串口接收 如果在中断里面开启接收，发送函数需要开关中断,具体原因请看文档UART里面关于 __HAL_LCOK 说明 */
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&huart1 , g_tBspUart1.pRxBuf , g_tBspUart1.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&huart1 , g_tBspUart1.pRxBuf , g_tBspUart1.rxSize);
				}
			}
			
			else if(huart->Instance == USART2)
			{//
				g_tBspUsart2.rxLen  = g_tBspUsart2.rxSize - rxCnt;
				g_tBspUsart2.rxFlag = 1;
				
				osEventFlagsSet(event_USART2_ID , 0x01U << 0);  /*启动事件标志组 event_Flags1的flag0置1*/				
				/* 重新开启串口接收 如果在中断里面开启接收，发送函数需要开关中断,具体原因请看文档UART里面关于 __HAL_LCOK 说明 */
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&husart2 , g_tBspUsart2.pRxBuf , g_tBspUsart2.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&husart2 , g_tBspUsart2.pRxBuf , g_tBspUsart2.rxSize);
				}
			}
			
			
			/* 串口4中断 */
			else if(huart->Instance == UART4)
			{
				g_tBspUart4.rxLen  = g_tBspUart4.rxSize - rxCnt;
				g_tBspUart4.rxFlag = 1;
				/* 重新开启串口接收 如果在中断里面开启接收，发送函数需要开关中断,具体原因请看文档UART里面关于 __HAL_LCOK 说明 */
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&huart4 , g_tBspUart4.pRxBuf , g_tBspUart4.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&huart4 , g_tBspUart4.pRxBuf , g_tBspUart4.rxSize);
				}
			}
			
			/* 串口6中断 */
			else if(huart->Instance == USART6)
			{
				g_tBspUsart6.rxLen  = g_tBspUsart6.rxSize - rxCnt;
				g_tBspUsart6.rxFlag = 1;
				/* 重新开启串口接收 如果在中断里面开启接收，发送函数需要开关中断,具体原因请看文档UART里面关于 __HAL_LCOK 说明 */
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&husart6 , g_tBspUsart6.pRxBuf , g_tBspUsart6.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&husart6 , g_tBspUsart6.pRxBuf , g_tBspUsart6.rxSize);
				}
			}	

			
		}
	}
}


