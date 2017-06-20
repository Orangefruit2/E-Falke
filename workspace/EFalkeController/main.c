#include "main.h"


int main(void) {

	initSerial();
	
	uint32_t last_Poll;
	
	//Init window watchdog
	WWDG_HandleTypeDef wwdg;
	WWDG_Init(&wwdg,HAL_RCC_GetHCLKFreq());
	
	while(1){
		// Polling alle 10ms 

		if(HAL_GetTick()-last_Poll > MILLIS_TO_TICK(10)) {
			pollControlMsg();
			checkReceiveState(rxCallback);
			last_Poll = HAL_GetTick();
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
