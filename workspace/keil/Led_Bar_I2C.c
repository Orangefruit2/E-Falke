#include <stm32f4xx_hal.h>
#include "Led_Bar_I2C.h"



void setI2CLed(I2C_HandleTypeDef* hi2c, uint8_t* data,int led, float brightness){
	//  led = max(1, min(10, led));
 // brightness = max(0, min(brightness, 1));

  // Zero based index 0-9 for bitwise operations
 // led--;

  // 8 (noticable) levels of brightness
  // 00000000 darkest
  // 00000011 brighter
  // ........
  // 11111111 brightest
	
  data[led] = ~(~0 << (unsigned char) (brightness*8));
	
	uint8_t send_data[20];
	for(int i=0;i<20;i++) {
		if(i%2==0) {
			send_data[i]=data[i/2];
		} else {
			send_data[i]=0;
		}
	}
	//sendI2C(hi2c,send_Data,10);
 // setData(ledBar);
}
void sendI2C(I2C_HandleTypeDef* hi2c, uint8_t* data, uint16_t size)
{ 
	uint8_t address = 0x00;
	HAL_I2C_Master_Transmit(hi2c,address, data,12,1000);
	
	uint8_t dat = 0x00;
	HAL_I2C_Master_Transmit(hi2c,address,&dat,1,1000);
	HAL_I2C_Master_Transmit(hi2c,address,&dat,1,1000);

  // Two extra empty bits for padding the command to the correct length
 // sendData(ledBar, 0x00);
 // sendData(ledBar, 0x00);

 // latchData(ledBar);
}