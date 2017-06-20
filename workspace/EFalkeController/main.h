#ifndef MAIN
#define MAIN
#include <stdbool.h>

#include "clock.h"
#include "watchdog.h"
#include "Serial/Serial.h"


void rxCallback(uint8_t uart, pi300_control_msg* msg);

#include "Display/Display.h"


#endif

