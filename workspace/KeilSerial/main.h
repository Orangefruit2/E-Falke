/* std */
#include <string.h>

/* Include core modules */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "pi300_control_msg.h"
#include "pi300_polling_msg.h"


#define BAUDRATE              38400
#define TXPIN1                 GPIO_PIN_6
#define RXPIN1                 GPIO_PIN_7
#define TXPIN2                 GPIO_PIN_5
#define RXPIN2                 GPIO_PIN_6
#define DATAPORT1              GPIOB
#define DATAPORT2              GPIOD
#define UART_PRIORITY         6
#define UART_RX_SUBPRIORITY   0

typedef struct{
uint8_t rxBuffer; // where we store that one character that just came in
pi300_control_msg controlMsgBuffer[2];	// Control Message from PI300, one for building, one is current state
uint8_t rxindex; // index for going through controlMsgBuffer while building
uint8_t currentMsgId;	// Index of the message in controlMsgBuffer which shows the current state
uint8_t flags;	// 1: New controlMsg was received
} rxControl;

/* rxControlStructs */
rxControl rx[2];		// Receive State for UART1 and UART2

/* Handle structs */
UART_HandleTypeDef huart1;	// Handle for UART1
UART_HandleTypeDef huart2;	// Handle for UART2
//IWDG_HandleTypeDef hiwdg;		// Watchdog

void initIO(void);
void initUSART(void);

void printS(char string[]);
void printB(uint8_t* bytes, uint8_t len);
