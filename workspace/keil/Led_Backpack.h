/*************************************************** 
  This is a library for our I2C LED Backpacks
  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/
  ----> http://www.adafruit.com/products/
  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#ifndef Led_Backpack_h
	#define Led_Backpack_h


#include <stm32f4xx_hal.h>


#include "Stdbool.h"


	#define LED_ON 1
	#define LED_OFF 0

	#define LED_RED 1
	#define LED_YELLOW 2
	#define LED_GREEN 3
 


	#define HT16K33_BLINK_CMD 0x80
	#define HT16K33_BLINK_DISPLAYON 0x01
	#define HT16K33_BLINK_OFF 0
	#define HT16K33_BLINK_2HZ  1
	#define HT16K33_BLINK_1HZ  2
	#define HT16K33_BLINK_HALFHZ  3

	#define HT16K33_CMD_BRIGHTNESS 0xE0

	#define SEVENSEG_DIGITS 5
	
	struct segment_display {
		uint16_t displaybuffer[8];
		I2C_HandleTypeDef *i2c;
	};
	

	void writeDigitNum(struct segment_display *display, uint8_t d, uint8_t num, bool dot);
	void writeDigitRaw(struct segment_display *display,uint8_t d, uint8_t bitmask);
	void clearDisplay(struct segment_display *display);
	void writeDisplay(struct segment_display *display, uint32_t timeout);
	void blinkRate(struct segment_display *display, uint8_t b,uint32_t timeout);
	void setBrightness(struct segment_display *display, uint8_t b,uint32_t timeout);

	
	#endif