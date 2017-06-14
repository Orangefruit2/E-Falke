#ifndef MAIN
#define MAIN

#include "clock.h"

#include "Serial/Serial.h"

//IWDG_HandleTypeDef hiwdg;		// Watchdog


void rxCallback(uint8_t uart, pi300_control_msg* msg);

#endif
