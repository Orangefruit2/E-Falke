#include "Display.h"
#include "Delay.h"

/*
	Write a single Segment with a digit between [0-9]
*/
static void writeInSegment(struct display *display, uint8_t digit,bool lightPoint) {
	switch(digit) {
		case 0:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_RESET); 
			break;
		case 1:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_RESET);
			break;
				case 2:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
				
						case 3:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
						
								case 4:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
								
										case 5:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
										
												case 6:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
												
														case 7:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_RESET);
			break;
														
																case 8:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
																		case 9:
			HAL_GPIO_WritePin(display->port,display->segments[0], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[1], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[3], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[4], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(display->port,display->segments[5], GPIO_PIN_SET);
			HAL_GPIO_WritePin(display->port,display->segments[6], GPIO_PIN_SET);
			break;
	}
	
	if(lightPoint) {
		HAL_GPIO_WritePin(display->port,display->segments[7], GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(display->port,display->segments[7], GPIO_PIN_RESET);
	}
	
}
/**
	Init the STM's IO Pins
*/
void init_display(struct display *display) {
	GPIO_InitTypeDef LED_Init;	
	uint16_t val=0;
	for(int i =0;i<4;i++) {
		val = val | display->digitPins[i];
	}
	for(int i =0;i<12;i++) {
		val = val | display->segments[i];
	}
	LED_Init.Mode=GPIO_MODE_OUTPUT_PP;
	LED_Init.Pin= val;
	LED_Init.Pull=GPIO_PULLDOWN;
	LED_Init.Speed=GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOD,&LED_Init);
}


/**
	Write a single Digit on the specific display number
*/
void writeDigit(struct display *display,void (*delay_Function_Ms)(uint32_t), uint8_t displayNumber, uint8_t digit, bool lightPoint) {
	if(displayNumber>=4) return;
	if(digit>=10) return;	
	HAL_GPIO_WritePin(GPIOD, display->digitPins[displayNumber], GPIO_PIN_SET);	//Enable the specific display
	writeInSegment(display,digit,lightPoint);
	delay_Function_Ms(1);// wait 1 ms 
	HAL_GPIO_WritePin(GPIOD, display->digitPins[displayNumber], GPIO_PIN_RESET); //Disable the specific display

	for(uint8_t i =0;i<7;i++) {
		HAL_GPIO_WritePin(display->port,display->segments[i], GPIO_PIN_RESET);
	}
}
/**

*/
void writeDoublePoint(struct display *display,void (*delay_Function_Ms)(uint32_t), bool lightDouble) {
	HAL_GPIO_WritePin(GPIOD, display->digitPins[4], GPIO_PIN_SET);	//Enable the specific display
	if(lightDouble) {
		HAL_GPIO_WritePin(display->port,display->segments[8], GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(display->port,display->segments[8], GPIO_PIN_RESET);
	}
	// wait 1 ms
	delay_Function_Ms(1);

	HAL_GPIO_WritePin(GPIOD, display->digitPins[4], GPIO_PIN_RESET);	//Disable the specific disable
	HAL_GPIO_WritePin(display->port,display->segments[7], GPIO_PIN_RESET);

}
