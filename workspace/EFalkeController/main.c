#include "main.h"


void init_Display_Main(struct display* display) {
	display->port=GPIOD;
	display->digitPins[0] =GPIO_PIN_15; //DISPLAY 1
	display->digitPins[1] =GPIO_PIN_13; //DISPLAY 2
	display->digitPins[2] =GPIO_PIN_11; //DISPLAY 3
	display->digitPins[3] =GPIO_PIN_9; //DISPLAY 4
	display->digitPins[4] =GPIO_PIN_0; //DISPLAY Double Point
	display->segments[0] = GPIO_PIN_10; //SEG A
	display->segments[1] = GPIO_PIN_12; //SEG B
	display->segments[2] = GPIO_PIN_14; //SEG C
	display->segments[3] = GPIO_PIN_8; //SEG D
	display->segments[4] = GPIO_PIN_3; //SEG E
	display->segments[5] = GPIO_PIN_5; //SEG F
	display->segments[6] = GPIO_PIN_1; //SEG G
	display->segments[7] = GPIO_PIN_7; //SEG Point
	display->segments[8] = GPIO_PIN_2; //SEG Double Point
	init_display(display);
}
int main(void) {

	initSerial();
	
	struct display display;
	init_Display_Main(&display);
	uint32_t led_Counter= 0;
	uint32_t last_Display = HAL_GetTick();
	uint32_t last_Poll = HAL_GetTick();
	bool doublePointLight = false;
	
	struct ledBar ledBar;
	ledBar.pinClock=GPIO_PIN_5;
	ledBar.pinData=GPIO_PIN_6;
	ledBar.pinPort=GPIOC;
	LED_Bar_init(&ledBar);
	
	//Init window watchdog
	WWDG_HandleTypeDef wwdg;
	WWDG_Init(&wwdg,HAL_RCC_GetHCLKFreq());
	
	while(1){
		// Polling alle 10ms 

		if(HAL_GetTick()-last_Poll > MILLIS_TO_TICK(10)) {
			pollControlMsg();
			checkReceiveState();
			last_Poll = HAL_GetTick();
		}
		
		if(HAL_GetTick()-last_Display>MILLIS_TO_TICK(750)) {
			if(led_Counter>=LED_COUNT){
				led_Counter = 0;
				for(int i =0;i<LED_COUNT;i++) {
					setLed(&ledBar,i,0); //Turn all LEDS off
				}
			}
			led_Counter=(led_Counter+1)%10;
			setLed(&ledBar,led_Counter,1);
			show_Data(&ledBar);
			last_Display = HAL_GetTick();
		}
		
		HAL_WWDG_Refresh(&wwdg); //Feed Watchdog. No Feed -> STM will reset.

	}
}

/* Callback function for new messages
 * Has to be passed to checkReceiveState
 * Reacts to new control messages
*/
void rxCallback(uint8_t uart, pi300_control_msg* msg){
	// TODO compute new Plane-State from new Msg at currentMsgId, update displays ...
	//printB(((uint8_t*)msg), sizeof(&msg));
}
