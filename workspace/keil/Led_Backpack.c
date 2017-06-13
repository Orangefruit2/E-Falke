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

#include "Led_Backpack.h"

static const uint8_t numbertable[] = {
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
};

void setBrightness(struct segment_display *display, uint8_t b,uint32_t timeout) {
  if (b > 15) b = 15;
	//	HAL_I2C_Init(display->i2c);
	//	HAL_I2C_Master_Transmit_IT(display->i2c,(0x70<<1),&b,sizeof(uint8_t));
//		HAL_I2C_DeInit(display->i2c);
}

void blinkRate(struct segment_display *display, uint8_t b,uint32_t timeout) {
		if (b > 3) b = 0; // turn off if not sure
	//	HAL_I2C_Init(display->i2c);
	//	HAL_I2C_Master_Transmit_IT(display->i2c,(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1))<<1,&b,sizeof(uint8_t));
	//	HAL_I2C_DeInit(display->i2c);
}
void writeDisplay(struct segment_display *display,uint32_t timeout) {
// HAL_I2C_Init(display->i2c);
	uint8_t val = 0x00;
 HAL_I2C_Master_Transmit_IT(display->i2c,(0x70<<1),&val,sizeof(uint8_t));

  for (uint8_t i=0; i<8; i++) {
		uint8_t first =(display->displaybuffer[i]&0xFF);
		uint8_t second =(display->displaybuffer[i] >> 8);
	
		HAL_I2C_Master_Transmit_IT(display->i2c,(0x70<<1),&first,sizeof(uint8_t));
		HAL_I2C_Master_Transmit_IT(display->i2c,(0x70<<1),&second,sizeof(uint8_t));
	}
//		HAL_I2C_DeInit(display->i2c);
}

void clearDisplay(struct segment_display *display) {
  for (uint8_t i=0; i<8; i++) {
		display->displaybuffer[i]=0;
  }
}
void writeDigitRaw(struct segment_display *display,uint8_t d, uint8_t bitmask) {
  if (d > 4) return;
  display->displaybuffer[d] = bitmask;
}
void writeDigitNum(struct segment_display *display, uint8_t d, uint8_t num, bool dot) {
  if (d > 4) return;
  writeDigitRaw(display,d, numbertable[num] | (dot << 7));
}