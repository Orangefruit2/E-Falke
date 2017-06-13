#ifndef DISPLAY_H
#define DISPLAY_H
#include <stm32f4xx_hal.h>
#include <stdbool.h>

#define SEGMENT_COUNT 9
#define DIGIT_COUNT 5  
struct display {
	GPIO_TypeDef *port;
	/*
		digitPins[0] -> First Digit on Display
		...
		digitPins[3] -> Last Digit on Display
		digitPins[4] -> Double Point 
	*/
	uint16_t digitPins[DIGIT_COUNT];
	/*
				--A--
			|				|
			F				B
			|				|
				--G--
			| 			|
		  E				C   -------
			|				|   -point-
				--D--     -------
	
		segments[0] = segment A			
		segments[1] = segment B				

		segments[2] = segment C
		segments[3] = segment D
		segments[4] = segment E
		segments[5] = segment F
		segments[6] = segment G
		segments[7] = segment point
		segments[8] = segment Double Point
	*/
	uint16_t segments[SEGMENT_COUNT];
};
void init_display(struct display *display);
void writeDigit(struct display *display,void (*delay_Function_Ms)(uint32_t), uint8_t displayNumber, uint8_t digit,bool lightPoint);
void writeDoublePoint(struct display *display,void (*delay_Function_Ms)(uint32_t), bool lightDouble);

#endif