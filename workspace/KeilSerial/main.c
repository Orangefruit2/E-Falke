#include "main.h"

int main(void) {
	
	initIO();
	initUSART();
	initDMA();
}

void initIO(){
	
	__GPIOB_CLK_ENABLE();
	__USART1_CLK_ENABLE();
	__DMA2_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = TXPIN | RXPIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(DATAPORT, &GPIO_InitStruct);
}

void initUSART(){

	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;		//Probably with parity
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);
	
	/* Print OK */
	//print("\r\n[OK] Serial started");

}

void initDMA(){
	

	hdma_usart1_rx.Instance = DMA2_Stream2;
	hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_DISABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_usart1_rx);

	__HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);

	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, UART_PRIORITY, UART_RX_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	
	
	/* Start the receiver */
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxBuffer, 1);
}

/* Prints the supplied string to uart */
void print(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
}

/* UART TX complete callback */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

/* UART RX complete callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1); // Clear the buffer to prevent overrun

	int i = 0;

	//print(&rxBuffer); // Echo the inputed character

	if (rxBuffer == 8 || rxBuffer == 127) // If Backspace or del
	{
		print(" \b"); // Properly clear the character
		rxindex--; 
		if (rxindex < 0) rxindex = 0;
	}

	else if (rxBuffer == '\n' || rxBuffer == '\r') // If Enter
	{
		///executeSerialCommand(rxString);
		rxString[rxindex] = 0;
		rxindex = 0;
		for (i = 0; i < MAXCLISTRING; i++) rxString[i] = 0; // Clear the string buffer
	}

	else
	{
		rxString[rxindex] = rxBuffer;
		rxindex++;
		if (rxindex > MAXCLISTRING)
		{
			rxindex = 0;
			for (i = 0; i < MAXCLISTRING; i++) rxString[i] = 0; // Clear the string buffer
			///print("\r\nBrinir> ");
		}
	}
}
