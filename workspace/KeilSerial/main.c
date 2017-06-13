#include "main.h"

int main(void) {
	
	initSerial();
	
	while(1){
		for(int i = 0;i<11000000/500*1000;i=i+1){
			pollControlMsg();
			checkReceiveState();
		}
	}
}


