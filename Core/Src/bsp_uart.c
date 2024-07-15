/**
  ******************************************************************************
  * File Name          : USART.c
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


/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"

/*	UART1 主要用于链接孙工的小园板*/
UART_HandleTypeDef 	huart1;
DMA_HandleTypeDef 	hdma_usart1_rx;
DMA_HandleTypeDef 	hdma_usart1_tx;

unsigned char g_uart1_rx_buf[USART1_RX_SIZE];
BspUart_t g_tBspUart1 = 
{
	.pRxBuf = g_uart1_rx_buf,
	.rxSize = USART1_RX_SIZE,
};

/*	UART4 主要用于链接上位机*/
UART_HandleTypeDef 	huart4;
DMA_HandleTypeDef 	hdma_uart4_rx;
DMA_HandleTypeDef 	hdma_uart4_tx;

unsigned char g_uart4_rx_buf[UART4_RX_SIZE];
BspUart_t g_tBspUart4 = 
{
	.pRxBuf = g_uart4_rx_buf,
	.rxSize = UART4_RX_SIZE,
};

/*	UART2 主要用于链接电池库仑计*/
UART_HandleTypeDef 	husart2;
DMA_HandleTypeDef 	hdma_usart2_rx;
DMA_HandleTypeDef 	hdma_usart2_tx;

unsigned char g_usart2_rx_buf[USART2_RX_SIZE];
BspUart_t g_tBspUsart2 = 
{
	.pRxBuf = g_usart2_rx_buf,
	.rxSize = USART2_RX_SIZE,
};


/*	USART6 主要用于控制相机*/
UART_HandleTypeDef 	husart6;
DMA_HandleTypeDef 	hdma_usart6_rx;
DMA_HandleTypeDef 	hdma_usart6_tx;

unsigned char g_usart6_rx_buf[USART6_RX_SIZE];
BspUart_t g_tBspUsart6 = 
{
	.pRxBuf = g_usart6_rx_buf,
	.rxSize = USART6_RX_SIZE,
};

/*
*********************************************************************************************************
*	函 数 名: HAL_UART_RxCpltCallback
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
//		//g_tBspUart1.rxFlag = 1;
//		//					/*启动事件标志组*/
//		//					/* 事件标志组event_Flags1的flag0置1 */
//		osEventFlagsSet(event_DYTDataF_ID , 0x01U << 0);  
	}
	else if(huart->Instance == USART2)
	{
		
	}	
	else if(huart->Instance == UART4)
	{
		
	}
	else if(huart->Instance == USART6)
	{
		
	}
	//g_tBspUart1.rxFlag = 1;
}



/*
*********************************************************************************************************
*	函 数 名: HAL_UART_TxCpltCallback
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
}


/*
*********************************************************************************************************
*	函 数 名: BSP_UART_Transmit_IT
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
						注意阻塞方式的发送函数不能通过开关中断开操作，避免使用阻塞方式
*********************************************************************************************************
*/
void BSP_UART_Transmit_IT(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size)
{
	while(huart->gState != HAL_UART_STATE_READY)
	{		
	}
	__disable_irq();
	HAL_UART_Transmit_IT(huart, pData, Size);
	__enable_irq();
}

/*
*********************************************************************************************************
*	函 数 名: BSP_UART_Transmit_DMA
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size)
{
	while(huart->gState != HAL_UART_STATE_READY)
	{		
	}
	__disable_irq();
	HAL_UART_Transmit_DMA(huart, pData, Size);
	__enable_irq();
}

/*
*********************************************************************************************************
*	函 数 名: HAL_UART_ErrorCallback
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	printf("HAL_UART_ErrorCallback \r\n");
}

/*
*********************************************************************************************************
*	函 数 名: MX_USART1_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
		printf("MX_USART1_UART_Init is fald/r/n");
  }
}

/*
*********************************************************************************************************
*	函 数 名: MX_USART1_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART2_UART_Init(void)
{

  husart2.Instance = USART2;
  husart2.Init.BaudRate = 4800;
  husart2.Init.WordLength = UART_WORDLENGTH_8B;
  husart2.Init.StopBits = UART_STOPBITS_1;
  husart2.Init.Parity = UART_PARITY_NONE;
  husart2.Init.Mode = UART_MODE_TX_RX;
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&husart2) != HAL_OK)
  {
		printf("MX_USART2_UART_Init is fald/r/n");
  }
}


/*
*********************************************************************************************************
*	函 数 名: MX_UART4_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_UART4_UART_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
		printf("MX_UART4_UART_Init is fald/r/n");
  }
}



/*
*********************************************************************************************************
*	函 数 名: MX_USART6_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART6_UART_Init(void)
{
  husart6.Instance = USART6;
  husart6.Init.BaudRate = 4800;
  husart6.Init.WordLength = UART_WORDLENGTH_8B;
  husart6.Init.StopBits = UART_STOPBITS_1;
  husart6.Init.Parity = UART_PARITY_NONE;
  husart6.Init.Mode = UART_MODE_TX_RX;
  husart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&husart6) != HAL_OK)
  {
		printf("MX_USART6_UART_Init is fald/r/n");
  }
}


/*
*********************************************************************************************************
*	函 数 名: HAL_UART_MspInit
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);
		
		/*DMA TX*/
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
		
	if(uartHandle->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle , hdmarx , hdma_usart2_rx);
		
		/*DMA TX*/
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle , hdmatx , hdma_usart2_tx);

    HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
	else if(uartHandle->Instance==UART4)
  {
    __HAL_RCC_UART4_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_uart4_rx.Instance = DMA1_Stream2;
    hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_rx.Init.Mode = DMA_NORMAL;
    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart4_rx);

		/*DMA TX*/
    hdma_uart4_tx.Instance = DMA1_Stream4;
    hdma_uart4_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_tx.Init.Mode = DMA_NORMAL;
    hdma_uart4_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle , hdmatx , hdma_uart4_tx);

    HAL_NVIC_SetPriority(UART4_IRQn , 1 , 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  }	
	else if(uartHandle->Instance==USART6)
  {
    __HAL_RCC_USART6_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_usart6_rx.Instance = DMA2_Stream1;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_NORMAL;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
    {
			printf("HAL_USART6_MspInit is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle , hdmarx , hdma_usart6_rx);

		/*DMA TX*/
    hdma_usart6_tx.Instance = DMA2_Stream6;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle , hdmatx , hdma_usart6_tx);

    HAL_NVIC_SetPriority(USART6_IRQn , 4 , 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  }
}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 反向初始化串口
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
	
	if(uartHandle->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
	
	if(uartHandle->Instance==UART4)
  {
    __HAL_RCC_UART4_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  }
	if(uartHandle->Instance==USART6)
  {
    __HAL_RCC_USART6_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
