#include "UART.h"
/* UART Handle structs */
UART_HandleTypeDef huart1;	// Handle for UART1
UART_HandleTypeDef huart2;	// Handle for UART2

void initUSART(UART_HandleTypeDef* huart, USART_TypeDef* Instance){
	huart->Instance = Instance;
	huart->Init.BaudRate = BAUDRATE;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(huart);
	
	/* Start the receiver */
		
		/* Peripheral interrupt init */

	if(Instance == USART1){
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		

		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	}else if(Instance == USART2){
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}

void initIO(){
	
	__GPIOB_CLK_ENABLE();
	__USART1_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__USART2_CLK_ENABLE();
	//__DMA2_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = TXPIN1 | RXPIN1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(DATAPORT1, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = TXPIN2 | RXPIN2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(DATAPORT2, &GPIO_InitStruct);
}
