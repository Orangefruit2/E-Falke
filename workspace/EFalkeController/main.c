#include "main.h"

int main(void) {
	
	
	
	initSerial();
	
	while(1){
		// Polling alle 10ms 
		HAL_Delay(MILLIS_TO_TICK(10));
		pollControlMsg();
		checkReceiveState();
	}
}

