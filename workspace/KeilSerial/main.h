/* std */
#include <string.h>

/* Include core modules */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "pi300_control_msg.h"


#define BAUDRATE              38400 //9600
#define TXPIN                 GPIO_PIN_6
#define RXPIN                 GPIO_PIN_7
#define DATAPORT              GPIOB
#define UART_PRIORITY         6
#define UART_RX_SUBPRIORITY   0
#define MAXCLISTRING          100 // Biggest string the user will type

uint8_t rxBuffer = '\000'; // where we store that one character that just came in
uint8_t rxString[MAXCLISTRING]; // where we build our string from characters coming in
int rxindex = 0; // index for going though rxString

/* Handle structs */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
//IWDG_HandleTypeDef hiwdg;		// Watchdog

void initIO(void);
void initUSART(void);
void initDMA(void);
