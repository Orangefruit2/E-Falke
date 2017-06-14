#include "Serial.h"

rxControl rx[2]; // Receive State for UART1 and UART2

void initSerial(){
	
	initIO();
	initUSART(&huart1, USART1);
	initUSART(&huart2, USART2);
}

void pollControlMsg(){
	pi300_polling_msg polling_msg = getExampleMsg();
	printB(((uint8_t*)&polling_msg), sizeof(polling_msg));
}

/* Checks whether new control messages have been received. If so, the flag is reset and 
 * the rxCallback function is called.
 */
void checkReceiveState(void (*rxCallback)(uint8_t uart, pi300_control_msg* msg)){

		for(int i = 0;i<2;i++){
			if(rx[i].flags & 0x1){
				rx[i].flags ^= 0x1;		// reset new Msg flag
				(*rxCallback)(i,&rx[i].controlMsgBuffer[rx[i].currentMsgId]);
			}
		}
}

/* Prints the supplied string to both uarts */
void printS(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
	HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 5);
}

/* Prints the supplied byte array of length len to both uarts */
void printB(uint8_t* bytes, uint8_t len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)bytes, len, 5);
	HAL_UART_Transmit(&huart2, (uint8_t*)bytes, len, 5);
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
	
	
	 // DEBUG printS(&rx[uart].rxBuffer); // Echo the inputed character
	
	// Beginne mit Empfang wenn die Adresse 121 empfangen wird oder setze den Empfang fort, wenn sie schon begonnen ist
	if((rx[uart].rxindex == 0 && rx[uart].rxBuffer == 121) || rx[uart].rxindex > 0){
			
		((uint8_t*)&rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2])[rx[uart].rxindex] = rx[uart].rxBuffer;
		rx[uart].rxindex++;
		
		if (rx[uart].rxindex > sizeof(rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2]))
		{
			rx[uart].rxindex = 0;
			if(checksumOK(&(rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2]))){
				rx[uart].flags |= 0x1;
				rx[uart].currentMsgId = (rx[uart].currentMsgId+1)%2;
			}else{
			//for (int i = 0; i < MAXCLISTRING; i++) rxString[i] = 0; // Clear the string buffer
			}
		}
	}
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
