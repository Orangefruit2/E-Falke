#ifndef UART_H
#define UART_H


/* Include core modules */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"


/* Serial config */

#define BAUDRATE              38400
#define TXPIN1                 GPIO_PIN_6
#define RXPIN1                 GPIO_PIN_7
#define TXPIN2                 GPIO_PIN_5
#define RXPIN2                 GPIO_PIN_6
#define DATAPORT1              GPIOB
#define DATAPORT2              GPIOD
#define UART_PRIORITY         6
#define UART_RX_SUBPRIORITY   0

extern UART_HandleTypeDef huart1;	// Handle for UART1
extern UART_HandleTypeDef huart2;	// Handle for UART2

void initIO(void);
void initUSART(UART_HandleTypeDef* huart, USART_TypeDef* Instance);

#endif
