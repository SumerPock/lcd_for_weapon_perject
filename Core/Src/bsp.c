/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32F407)
*	文件名称 : bsp.c
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
#include "bsp.h"



/*
*********************************************************************************************************
*	                                   函数声明
*********************************************************************************************************
*/
//static void SystemClock_Config(void);
void Error_Handler(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
//	/* 
//       STM32H407 HAL 库初始化，此时系统用的还是F407自带的16MHz，HSI时钟:
//	   - 调用函数HAL_InitTick，初始化滴答时钟中断1ms。
//	   - 设置NVIV优先级分组为4。
//	 */
//	HAL_Init();

//	/* 
//       配置系统时钟到168MHz
//       - 切换使用HSE。
//       - 此函数会更新全局变量SystemCoreClock，并重新配置HAL_InitTick。
//    */
//	SystemClock_Config();

//	/* 
//	   Event Recorder：
//	   - 可用于代码执行时间测量，MDK5.25及其以上版本才支持，IAR不支持。
//	   - 默认不开启，如果要使能此选项，务必看V5开发板用户手册第8章
//	*/	
//#if Enable_EventRecorder == 1  
//	/* 初始化EventRecorder并开启 */
//	EventRecorderInitialize(EventRecordAll, 1U);
//	EventRecorderStart();
//#endif
//	
//	bsp_InitKey();    	/* 按键初始化，要放在滴答定时器之前，因为按钮检测是通过滴答定时器扫描 */
//	bsp_InitTimer();  	/* 初始化滴答定时器 */
//	bsp_InitUart();		/* 初始化串口 */
//	bsp_InitLed();    	/* 初始化LED */	
//	BEEP_InitHard();    /* 初始化蜂鸣器 */
//	
//	/* 针对不同的应用程序，添加需要的底层驱动模块初始化函数 */

	
//	bsp_InitSFlash();	/* 初始化SPI 串行Flash */
}


/* 软件看门狗
 * 设置 IWDG 的超时时间
 * Tout = prv/40 * rlv (s)
 *      prv可以是[4,8,16,32,64,128,256]
 * prv:预分频器值，取值如下：
 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
 *
 *        独立看门狗使用LSI作为时钟。
 *        LSI 的频率一般在 30~60KHZ 之间，根据温度和工作场合会有一定的漂移，我
 *        们一般取 40KHZ，所以独立看门狗的定时时间并一定非常精确，只适用于对时间精度
 *        要求比较低的场合。
 *
 * rlv:重装载寄存器的值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出 
 *                        （625/40）*625 = 4s
 */
void MX_IWDG_Init(IWDG_HandleTypeDef *idwg)
{
	idwg->Instance = IWDG;
	idwg->Init.Prescaler = IWDG_PRESCALER_256;
	idwg->Init.Reload = 625;
	if(HAL_IWDG_Init(idwg)!= HAL_OK)
	{
		Error_Handler();
	}  
}



/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些处理时间要求不严格的
*			任务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
//	bsp_KeyScan10ms();
//	BEEP_Pro();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的事务
*			 可以放在此函数。比如：触摸坐标扫描。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- 喂狗 */

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
}

unsigned char bsp_sun_protocol(unsigned char *ucucStatus , unsigned char *ucucCount);

unsigned char DYTCommData(unsigned char* buff , unsigned char buffled)
{
	static int loop = 0;
	
	unsigned char ucreturn = 0;
	if(*buff == 0x55 && *(buff + 1) == 0xAA && *(buff + 2) == 0xDC)
	{
		printf("loop = %d \r\n" , loop);
		loop++;
	}
	return ucreturn;
}


unsigned char bsp_sun_protocol(unsigned char *ucucStatus , unsigned char *ucucCount)
{
	static unsigned char Buff[64] = {0};
	unsigned char ucreturn = 0;
	unsigned char read = 0;
	static int loop = 0;
	//static struct BUTTERY buttery;
	osStatus_t os_Status; 
	//if (comGetChar(COM1 , &read))
	{
		loop++;
		printf("loop = %d \r\n" , loop);
		*ucucStatus = 0;
		*ucucCount=0;			
//		switch (*ucucStatus)
//		{
//			/* 状态0保证接收到0x55 */
//			case 0:
//				if(read == 0x55)
//				{
//					*ucucStatus = 1;        
//				}
//				break;

//			/* 状态1保证接收到0xAA */
//			case 1:
//				if(read == 0xAA)
//				{
//					*ucucStatus = 2;//1Byte数据
//				}
//				else
//				{
//					*ucucStatus = 0;
//				}
//				break;
//				
//			/* 状态2保证接收到0xDC */	
//			case 2:
//				if(read == 0xDC)
//				{
//					*ucucStatus = 3;
//				}
//				else
//				{
//					*ucucStatus = 0;
//				}
//				break;				
//			
//			case 3:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 21)			/* 接收够1个数据 */
//				{
//					/*启动事件标志组*/
//					/* 事件标志组event_Flags1的flag0置1 */
//					osEventFlagsSet(event_DYTDataF_ID , 0x01U << 0);  
//					loop++;
//					printf("loop = %d \r\n" , loop);
////					unsigned char buff1[26] = {0x55 , 0xaa ,0xdc};
////					for(unsigned char i = 0 ; i <24 ; i++ )
////					{
////						buff1[i + 3] = Buff[i];
////					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					//comSendBuf(COM4 , buff1 , 25);
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}
//				break;
//							
//			default:
//				break;
//		}
	}
	return ucreturn;
}

unsigned char bsp_buttery_protocol(unsigned char *getbuff , unsigned char getbuffsize)
{
	unsigned char ucreturn = 0;
	static unsigned char Buff[32] = {0};
	static unsigned char ButteryData[16] = {0x55 , 0xAA};
	static unsigned char ButteryDataflag[6] = {0};	
	static struct BUTTERY buttery;
	osStatus_t os_Status; 
	for(unsigned char i = 0 ; i < getbuffsize ; i ++)
	{
		Buff[i] = *(getbuff + i);
	}
	if(Buff[0] == 0xff && Buff[1] == 0x02)
	{
		switch(Buff[2])
		{
			case 0x08://电池温度
				buttery.temperature = Buff[3];
				ButteryDataflag[0] = 1;
				memset(Buff, 0, sizeof(Buff));
				break;
	
			case 0x16://电池状态
				buttery.ButryStae = Buff[3];
				ButteryDataflag[1] = 1;
				memset(Buff, 0, sizeof(Buff));
				break;

			case 0x0D://电池相对百分比
				buttery.Butryremaining = Buff[3];
				ButteryDataflag[2] = 1;
				memset(Buff, 0, sizeof(Buff));
				break;
			
			default:
				ucreturn = 4;
				memset(Buff, 0, sizeof(Buff));
				break;
		}
	}
	else if(Buff[0] == 0xff && Buff[1] == 0x03)
	{	
		switch(Buff[2])
		{
			case 0x09://电压
				buttery.Voltage = Buff[4] | (Buff[3]<<8);
				ButteryDataflag[3] = 1;
				memset(Buff, 0, sizeof(Buff));
				break;

			case 0x0F:
				buttery.Butrycapacity = Buff[4] | (Buff[3]<<8);
				ButteryDataflag[4] = 1;
				memset(Buff, 0, sizeof(Buff));
				break;
	
			case 0x0A://剩余容量
				buttery.Electric_current = Buff[4] | (Buff[3]<<8);
				ButteryDataflag[5] = 1;
				memset(Buff, 0, sizeof(Buff));
				break;
			
			default:
				ucreturn = 3;
				memset(Buff, 0, sizeof(Buff));
				break;
		}
	}	
	if(ButteryDataflag[0] + ButteryDataflag[1] + ButteryDataflag[2] + ButteryDataflag[3] + ButteryDataflag[4] 
		 + ButteryDataflag[5] == 6)
	{
		os_Status = osMessageQueuePut(msgQueue_ButteryData, /* 消息队列的名字 */
								&buttery, 									/* 放入消息队列的变量（消息） */
								NULL,              											/* 放入消息的优先级 */
								NULL               											/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
								);
		osEventFlagsSet(event_Battery_ID , 0x01U << 0);  /*启动事件标志组 event_Flags1的flag0置1*/			
		memset(Buff, 0, sizeof(Buff));
		memset(ButteryDataflag, 0, sizeof(ButteryDataflag));
		memset(&buttery, 0, sizeof(buttery));
		
	}	
	return ucreturn;
}

//unsigned char bsp_buttery_protocol(unsigned char *ucucStatus , unsigned char *ucucCount)
//{
//	static unsigned char Buff[32] = {0};
//	static unsigned char ButteryData[16] = {0x55 , 0xAA};
//	static unsigned char ButteryDataflag[6] = {0};
//	unsigned char ucreturn = 0;
//	unsigned char read = 0;
//	static struct BUTTERY buttery;
//	osStatus_t os_Status; 
//	//if (comGetChar(COM2, &read))
//	{
//		//printf("%X\r\n", read);
//		switch (*ucucStatus)
//		{
//			/* 状态0保证接收到0xff */
//			case 0:
//				if(read == 0xFF)
//				{
//					*ucucStatus = 1;        
//				}
//				break;

//			/* 状态1保证接收到0x02或0x03 */
//			case 1:
//				if(read == 0x02)
//				{
//					*ucucStatus = 2;//1Byte数据
//				}
//				else if(read == 0x03)
//				{
//					*ucucStatus = 3;//2Byte数据
//				}
//				else
//				{
//					*ucucStatus = 0;
//				}
//				break;

//			case 2:
//				switch(read)
//				{
//					case 0x16://电池状态
//						*ucucStatus = 5;
//						break;
//					
//					case 0x08://温度
//						*ucucStatus = 6;
//						break;

//					case 0x0D://剩余百分比
//						*ucucStatus = 9;
//						break;
//						
//					default:
//						*ucucStatus = 0;
//						break;
//				}
//				break;

//				case 3:
//					switch(read)
//					{
//						case 0x09://电压
//						*ucucStatus = 7;
//						break;

//						case 0x0A://电刘
//						*ucucStatus = 8;
//						break;

//						case 0x0F://剩余电池容量
//						*ucucStatus = 10;
//						break;

//						default:
//						*ucucStatus = 0;
//						break;
//					}
//					break;
//				
//			/*状态*/
//			case 5:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 1)			/* 接收够1个数据 */
//				{
//					unsigned char temp = 0;	
//					buttery.ButryStae = Buff[0];
//					buttery.add = Buff[1];
//					temp = 0xff + 0x02 + 0x16 + buttery.ButryStae;
//					if(buttery.add == temp)
//					{		
//						ButteryData[2] = 	0x16;	
//						ButteryData[3] = 	buttery.ButryStae;	
//						if(ButteryDataflag[0] == 0)
//						{
//							ButteryDataflag[0]++;
//						}					
//					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}
//				break;
//				
//			/*温度*/
//			case 6:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 1)			/* 接收够1个数据 */
//				{
//					unsigned char temp = 0;	
//					buttery.temperature = Buff[0];
//					buttery.add = Buff[1];
//					temp = 0xff + 0x02 + 0x08 + buttery.temperature;
//					if(buttery.add == temp)
//					{	
//						ButteryData[4] = 	0x08;	
//						ButteryData[5] = 	buttery.temperature;			
//						if(ButteryDataflag[1] == 0)
//						{
//							ButteryDataflag[1]++;
//						}				
//					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}						
//				break;

//			/*电压*/
//			case 7:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 2)			/* 接收够2个数据 */
//				{
//					unsigned char temp = 0;	
//					buttery.Voltage = Buff[1] | (Buff[0]<<8);
//					buttery.add = Buff[2];
//					temp =  0xff + 0x03 + 0x09 + Buff[1] + Buff[0];
//					if(temp == buttery.add)
//					{
//						ButteryData[6] = 	0x09;	
//						ButteryData[7] = 	buttery.Voltage;
//						ButteryData[8] = 	buttery.Voltage>>8;
//						if(ButteryDataflag[2] == 0)
//						{
//							ButteryDataflag[2]++;
//						}	
//					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}						
//				break;	
//			
//			/*电流*/
//			case 8:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 2)			/* 接收够2个数据 */
//				{
//					unsigned char temp = 0;	
//					buttery.Electric_current = Buff[1] | (Buff[0]<<8);//充电为正数，放电为负数
//					buttery.add = Buff[2];
//					temp =  0xff + 0x03 + 0x0A + Buff[1] + Buff[0];
//					if(temp == buttery.add)
//					{
//						ButteryData[6] = 	0x0A;	
//						ButteryData[7] = 	buttery.Electric_current;
//						ButteryData[8] = 	buttery.Electric_current>>8;
//						if(ButteryDataflag[3] == 0)
//						{
//							ButteryDataflag[3]++;
//						}	
//					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}						
//				break;	
//			
//			/*剩余百分比*/	
//			case 9:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 1)	
//				{
//					unsigned char temp = 0;	
//					buttery.Butryremaining = Buff[0];
//					buttery.add = Buff[1];
//					temp = 0xff + 0x02 + 0x0D + buttery.Butryremaining;
//					if(buttery.add == temp)
//					{						
//						ButteryData[9] = 	0x0D;	
//						ButteryData[10] = buttery.Butryremaining;
//						if(ButteryDataflag[4] == 0)
//						{
//							ButteryDataflag[4]++;
//						}								
//					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}						
//				break;	
//			
//			/*剩余容量*/
//			case 10:
//				Buff[(*ucucCount)] = read;
//				if((*ucucCount) == 2)			
//				{
//					unsigned char temp = 0;	
//					buttery.Butrycapacity = Buff[1] | (Buff[0]<<8);
//					buttery.add = Buff[2];
//					temp = 0xFF + 0x03 + 0x0F + Buff[1] + Buff[0];
//					if(temp == buttery.add)
//					{
//						ButteryData[9] = 	0x0F;
//						ButteryData[10] = buttery.Butrycapacity;							
//						ButteryData[11] = buttery.Butrycapacity>>8;
//						if(ButteryDataflag[5] == 0)
//						{
//							ButteryDataflag[5]++;
//						}	
//					}
//					*ucucStatus = 0;
//					*ucucCount=0;
//					memset(Buff, 0, sizeof(Buff));
//				}
//				else
//				{
//					(*ucucCount)++;
//				}						
//				break;	
//				
//			default:
//				break;
//		}
//	}	
//	if(ButteryDataflag[0] + ButteryDataflag[1] + ButteryDataflag[2] + ButteryDataflag[3] + ButteryDataflag[4] 
//		 + ButteryDataflag[5] == 6)
//	{
//		os_Status = osMessageQueuePut(msgQueue_ButteryData, /* 消息队列的名字 */
//								&buttery, 									/* 放入消息队列的变量（消息） */
//								NULL,              											/* 放入消息的优先级 */
//								NULL               											/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
//								);
//		memset(Buff, 0, sizeof(Buff));
//		memset(ButteryDataflag, 0, sizeof(ButteryDataflag));
//		memset(&buttery, 0, sizeof(buttery));
//		
//	}
//	return ucreturn;
//}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/*
*********************************************************************************************************
*	函 数 名: HAL_Delay
*	功能说明: 重定向毫秒延迟函数。替换HAL中的函数。因为HAL中的缺省函数依赖于Systick中断，如果在USB、SD卡
*             中断中有延迟函数，则会锁死。也可以通过函数HAL_NVIC_SetPriority提升Systick中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
/* 当前例子使用stm32f4xx_hal.c默认方式实现，未使用下面重定向的函数 */
#if 0
void HAL_Delay(uint32_t Delay)
{
	bsp_DelayUS(Delay * 1000);
}
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
