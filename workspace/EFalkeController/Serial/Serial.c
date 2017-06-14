/**
 *	This part of the software for the serial communication with the two PI300 handles receiving and
 *	sending data between the PI300s and the stm uController.
 *	It handles the required Interrupts, allows sending Strings and byte arrays to the PI300s and
 *	polling of state information of the battery/motor.
 *  It keeps track of received/ing control messages and allows to execute callback functions when
 *	new messages have been received.
 *
 *	@author=Matthias Kraus
 *	@mail=mak4293@thi.de
 */

#include "Serial.h"

rxControl rx[2]; // Receive State for UART1 and UART2

/*	Inits all the used GPIOs and UARTs. Has to be called once before sending and receiving is possible */
void initSerial(){
	
	initIO();
	initUSART(&huart1, USART1);
	initUSART(&huart2, USART2);
}

/*	Sends a polling message to both PI300s */
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

/* Prints the supplied string to both PI300s */
void printS(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
	HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 5);
}

/* Prints the supplied byte array of length len to both PI300s */
void printB(uint8_t* bytes, uint8_t len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)bytes, len, 5);
	HAL_UART_Transmit(&huart2, (uint8_t*)bytes, len, 5);
}

/* UART RX complete callback
 *	Handles received bits from both UARTS/PI300s, stores them in the buffer, checks message integrity by
 *	checksum and sets the new message flag when a new message is correctly received.
 */
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
		
	// Start buffering a new message wenn address 121 (ASCII y) is received or continue when buffering is already in progress
	if((rx[uart].rxindex == 0 && rx[uart].rxBuffer == 121) || rx[uart].rxindex > 0){
			
		((uint8_t*)&rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2])[rx[uart].rxindex] = rx[uart].rxBuffer;
		rx[uart].rxindex++;
		
		
		if (rx[uart].rxindex > sizeof(rx[uart].controlMsgBuffer[(rx[uart].currentMsgId+1)%2]))
		{
			// If message is completed, reset message write index
			rx[uart].rxindex = 0;
			
			// If message checksum is ok, set new message flag and point currentMsgId to the new message
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
	//printS("[ERROR] Serial Error\n");
}
