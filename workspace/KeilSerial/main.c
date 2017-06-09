#include "main.h"

int main(void) {
	
//	HAL_Init();
	
	initIO();
	initUSART();
	
	
	pi300_polling_msg polling_msg = getExampleMsg();
	while(1){
		for(int i = 0;i<11000000/500*1000;i=i+1);
		printB(((uint8_t*)&polling_msg), sizeof(polling_msg));
		// Check receive states
		for(int i = 0;i<2;i++){
			if(rx[i].flags & 0x1){
				// compute new Plane-State from new Msg at currentMsgId
				printB(((uint8_t*)&rx[i].controlMsgBuffer[rx[i].currentMsgId]), sizeof(rx[i].controlMsgBuffer[rx[i].currentMsgId]));
			}
		}
	}
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

void initUSART(){

	//===============USART1==============
	
	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);
	
	/* Print OK */
	printS("\r\n[OK] Serial1 started");
	

	/* Start the receiver */
		
		/* Peripheral interrupt init */

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	

  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	
	//=========USART2====================
	
	huart2.Instance = USART2;
	huart2.Init.BaudRate = BAUDRATE;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart2);
	
	/* Print OK */
	printS("\r\n[OK] Serial2 started");
	

	/* Start the receiver */
		
		/* Peripheral interrupt init */

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	

  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

}


/* Prints the supplied string to uart */
void printS(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
	HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 5);
}

void printB(uint8_t* bytes, uint8_t len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)bytes, len, 5);
	HAL_UART_Transmit(&huart2, (uint8_t*)bytes, len, 5);
}


void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
	HAL_UART_Receive_IT(&huart1, &(rx[0].rxBuffer), 1);
  __HAL_UART_FLUSH_DRREGISTER(&huart1);
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
	HAL_UART_Receive_IT(&huart2, &(rx[1].rxBuffer), 1);
  __HAL_UART_FLUSH_DRREGISTER(&huart2);
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
	__HAL_UART_FLUSH_DRREGISTER(huart); // Clear the buffer to prevent overrun

	
	//Check from which UART the byte was received
	uint8_t uart;
	if(huart == &huart1){
		uart = 0;
	}else{
		uart = 1;
	}
	
	
	 printS(&rx[uart].rxBuffer); // Echo the inputed character
	
	// Beginne mit Empfang wenn die Adresse 121 empfangen wird oder setze den Empfang fort, wenn sie schon begonnen ist
	if((rx[uart].rxindex == 0 && rx[uart].rxBuffer == 121) || rx[uart].rxindex > 0){
			
		((uint8_t*)&rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2])[rx[uart].rxindex] = rx[uart].rxBuffer;
		rx[uart].rxindex++;
		
		if (rx[uart].rxindex > sizeof(rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2]))
		{
			rx[uart].rxindex = 0;
			rx[uart].flags |= 0x1;
			rx[uart].currentMsgId = (rx[uart].currentMsgId+1)%2;
			//for (int i = 0; i < MAXCLISTRING; i++) rxString[i] = 0; // Clear the string buffer
		}
		
	}
}
