
#include <stm32f4xx_hal.h>
#define TICK_RATE 1000
#define MILLIS_TO_TICK(X) ((uint32_t) (X* TICK_RATE/1000.00))
#define MICROS_TO_TICK(X) ((uint32_t) (X* TICK_RATE/1000000.00));

void SystemClock_Config(void);
