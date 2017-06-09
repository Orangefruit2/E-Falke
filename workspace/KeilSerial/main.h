/* std */
#include <string.h>

/* Include core modules */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "pi300_control_msg.h"
#include "pi300_polling_msg.h"


#define BAUDRATE              38400 //9600
#define TXPIN                 GPIO_PIN_6
#define RXPIN                 GPIO_PIN_7
#define DATAPORT              GPIOB
#define UART_PRIORITY         6
#define UART_RX_SUBPRIORITY   0

uint8_t rxBuffer = '\000'; // where we store that one character that just came in
pi300_control_msg controlMsgBuffer[2];	// Control Message from PI300, one for building, one is current state
uint8_t rxindex = 0; // index for going through controlMsgBuffer while building
uint8_t currentMsgId = 0;	// Index of the message in controlMsgBuffer which shows the current state
uint8_t flags = 0;	// 1: New controlMsg was received

/* Handle structs */
UART_HandleTypeDef huart1;
//IWDG_HandleTypeDef hiwdg;		// Watchdog

void initIO(void);
void initUSART(void);

void printS(char string[]);
void printB(uint8_t* bytes, uint8_t len);