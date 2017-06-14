#include "main.h"

int main(void) {
	
	
	
	initSerial();
	
	while(1){
		// Polling alle 10ms 
		HAL_Delay(MILLIS_TO_TICK(10));
		pollControlMsg();
		checkReceiveState(rxCallback);
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
