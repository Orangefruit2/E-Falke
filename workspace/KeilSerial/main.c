#include "main.h"

int main(void) {
	
//	HAL_Init();
	
	initIO();
	initUSART();
	
	while(1);
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
	printS("\r\n[OK] Serial started");
	

	/* Start the receiver */
		
		/* Peripheral interrupt init */

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	

  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

}


/* Prints the supplied string to uart */
void printS(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
	HAL_UART_Receive_IT(&huart1, &rxBuffer, 1);
  __HAL_UART_FLUSH_DRREGISTER(&huart1);
}


/* UART TX complete callback */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	printS("[ERROR] Serial Error\n");
	
}


/* UART RX complete callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1); // Clear the buffer to prevent overrun

	int i = 0;

	printS(&rxBuffer); // Echo the inputed character
	/*
	if (rxBuffer == 8 || rxBuffer == 127) // If Backspace or del
	{
		printS(" \b"); // Properly clear the character
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
			printS("\r\nBrinir> ");
		}
	}
	*/
	
	//HAL_UART_Receive_IT(&huart1, &rxBuffer, 1);
	
}
