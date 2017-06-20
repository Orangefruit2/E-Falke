#ifndef LED_BAR_H
#define LED_BAR_H

#include <stm32f4xx_hal.h>

// Avoid name conflict
#define GLB_CMDMODE 0x00  // Work on 8-bit mode
#define GLB_ON      0xff  // 8-bit 1 data
#define GLB_OFF     0x00  // 8-bit 0 data
#define LED_COUNT 10

typedef struct ledBar {
	uint16_t pinClock;
  uint16_t pinData;
	GPIO_TypeDef* pinPort;
	uint8_t state[LED_COUNT];
} ledBar;

void LED_Bar_init(ledBar* bar);
void setLed(ledBar* ledBar,uint8_t led, float brightness);// Set brightness for a single led, range from 0 to 1
void show_Data(ledBar* ledBar);

#endif