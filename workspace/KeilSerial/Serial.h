#ifndef SERIAL_H
#define SERIAL_H

/* std */
#include <string.h>

#include "UART.h"


/* Include message definitions */
#include "pi300_control_msg.h"
#include "pi300_polling_msg.h"


typedef struct{
uint8_t rxBuffer; // where we store that one character that just came in
pi300_control_msg controlMsgBuffer[2];	// Control Message from PI300, one for building, one is current state
uint8_t rxindex; // index for going through controlMsgBuffer while building
uint8_t currentMsgId;	// Index of the message in controlMsgBuffer which shows the current state
uint8_t flags;	// 1: New controlMsg was received
} rxControl;

/* rxControlStructs */
extern rxControl rx[2];		// Receive State for UART1 and UART2

void initSerial(void);

void pollControlMsg(void);
void checkReceiveState(void);

void printS(char string[]);
void printB(uint8_t* bytes, uint8_t len);

#endif
