#include "bsp.h"


void ConfigOUT(void)
{	
	GPIO_InitTypeDef  GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);		
}

void ConfigSP330(enum SP330NUM sn , enum SP330MODE sm);

void ConfigLed(void)
{
	int ireturn = 0;
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_4;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
	
	
	GPIO_InitStruct.Pin       = GPIO_PIN_5;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin       = GPIO_PIN_5;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);		
}

//sp330配置
void ConfigSP330(enum SP330NUM sn , enum SP330MODE sm)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	switch(sn)
	{
		case U1:
			GPIO_InitStruct.Pin   = GPIO_PIN_15;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			GPIO_InitStruct.Pin   = GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_5;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);		
		
			GPIO_InitStruct.Pin   = GPIO_PIN_12;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
		
			HAL_GPIO_WritePin(GPIOC , GPIO_PIN_12 , GPIO_PIN_SET); //SHDN 	低电平时启动低功耗
			HAL_GPIO_WritePin(GPIOA , GPIO_PIN_15 , GPIO_PIN_SET); //SLEW		低电平时限制速度250K
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_1 , GPIO_PIN_RESET);//485_EN	低电平时启动485接收
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_0 , GPIO_PIN_SET);//T2IN, DE 高电平时启动驱动程序
			switch(sm)
			{
				case RS232:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_5 , GPIO_PIN_RESET); //485/232_3
				break;

				case RS422:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_5 , GPIO_PIN_SET);		//485/422模式
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);//全双工
				break;

				case RS485:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_5 , GPIO_PIN_SET);		//485/422模式
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);//全双工
				break;		
			}
		break;
		
		case U7:
			switch(sm)
			{
				case RS232:
				break;

				case RS422:
				break;

				case RS485:
				break;		
			}
			break;
		
		case U8:
			GPIO_InitStruct.Pin   = GPIO_PIN_8;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			GPIO_InitStruct.Pin   = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);		
		
			HAL_GPIO_WritePin(GPIOA , GPIO_PIN_8 , GPIO_PIN_SET); //SHDN 	低电平时启动低功耗
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_15 , GPIO_PIN_SET); //SLEW		低电平时限制速度250K
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_12 , GPIO_PIN_RESET);//485_EN	低电平时启动485接收
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_11 , GPIO_PIN_SET);//T2IN, DE 高电平时启动驱动程序
			switch(sm)
			{
				case RS232:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_14 , GPIO_PIN_RESET); //485/232_2
				break;

				case RS422:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_14 , GPIO_PIN_SET);		//485/422模式
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET);//全双工
				break;

				case RS485:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_14 , GPIO_PIN_SET);		//485/422模式
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET);//全双工
				break;		
			}
			break;	

		case U26:
			GPIO_InitStruct.Pin   = GPIO_PIN_8;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);		
		
			GPIO_InitStruct.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);		
		
			GPIO_InitStruct.Pin   = GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_9;
			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
		
			HAL_GPIO_WritePin(GPIOE , GPIO_PIN_1 , GPIO_PIN_SET); 	//SHDN 	低电平时启动低功耗
			HAL_GPIO_WritePin(GPIOE , GPIO_PIN_0 , GPIO_PIN_SET); 	//SLEW		低电平时限制速度250K
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_7 , GPIO_PIN_RESET);	//485_EN	低电平时启动485接收
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_6 , GPIO_PIN_SET);		//T2IN, DE 高电平时启动驱动程序 422_3
			switch(sm)
			{
				case RS232:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_5 , GPIO_PIN_RESET); 
				break;

				case RS422:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_5 , GPIO_PIN_SET); 
					HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_RESET);
				break;

				case RS485:
					HAL_GPIO_WritePin(GPIOD , GPIO_PIN_5 , GPIO_PIN_SET); 
					HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_RESET);
				break;	

				default:
					break;
			}
			break;	


			default:
			break;			
	}
}