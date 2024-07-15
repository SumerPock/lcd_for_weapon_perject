/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "bsp.h"

#define USART1_RX_SIZE	128
#define USART2_RX_SIZE	128
	 
#define UART4_RX_SIZE	128
#define USART6_RX_SIZE	128
	 
	 
	 
 
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle);	 
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle);	 
void MX_UART4_UART_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART6_UART_Init(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);	 
void BSP_UART_Transmit_IT(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size);
void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart); 
	 
	 
extern unsigned char g_uart1_rx_buf[USART1_RX_SIZE];
extern unsigned char g_usart2_rx_buf[USART2_RX_SIZE];
extern unsigned char g_usart6_rx_buf[USART6_RX_SIZE];
extern unsigned char g_uart4_rx_buf[UART4_RX_SIZE];

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef husart2;
extern UART_HandleTypeDef husart6;

extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;

extern DMA_HandleTypeDef 	hdma_usart2_rx;
extern DMA_HandleTypeDef 	hdma_usart2_tx;

extern DMA_HandleTypeDef 	hdma_usart6_rx;
extern DMA_HandleTypeDef 	hdma_usart6_tx;

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
