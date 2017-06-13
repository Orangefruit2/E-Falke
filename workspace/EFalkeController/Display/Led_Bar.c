#include "LED_Bar.h"

static void LED_Bar_Init_Pin(int pin) {
	//INit a single Pin 
	GPIO_InitTypeDef LED_Init;
	LED_Init.Mode=GPIO_MODE_OUTPUT_PP;
	LED_Init.Pin= pin;
	LED_Init.Pull=GPIO_PULLDOWN;
	LED_Init.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD,&LED_Init);
}


// Send the latch command
static void latchData(ledBar* ledBar) {
			HAL_GPIO_WritePin(GPIOD, ledBar->pinData, GPIO_PIN_RESET);
			HAL_Delay(1);
	
			for (unsigned char i = 0; i < 4; i++)
			{
				HAL_GPIO_WritePin(GPIOD, ledBar->pinData, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, ledBar->pinData, GPIO_PIN_RESET);
			}
}

// Send 16 bits of data
static void sendData(ledBar* ledBar, unsigned int data) {
  for (unsigned char i = 0; i < 16; i++)
  {
    unsigned int state = (data & 0x8000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		
		HAL_GPIO_WritePin(GPIOD, ledBar->pinData, state);
		
		state = HAL_GPIO_ReadPin(GPIOD,ledBar->pinClock);
    state = state ? GPIO_PIN_RESET : GPIO_PIN_SET; //Toogle Pin
		HAL_GPIO_WritePin(GPIOD, ledBar->pinClock, state);

    data <<= 1;
  }
}
void LED_Bar_init(ledBar* bar) {
			//All Leds off
		for (int i = 0; i < 10; i++){
			bar->state[i] = 0x00;
		}
		//Init clockPin
		LED_Bar_Init_Pin(bar->pinClock);
		//Init DataPin
		LED_Bar_Init_Pin(bar->pinData);
}
void setLed(ledBar* ledBar,uint8_t led, float brightness){
	
  if(brightness>1)brightness=1;
	if(brightness<0)brightness=0;
	
	if(led>9)led=9;

  // 8 (noticable) levels of brightness
  // 00000000 darkest
	// 00000001 brighter
  // 00000011 brighter
	// 00000111 brighter
  // ........
  // 11111111 brightest
  ledBar->state[led] = ~(~0 << (uint8_t) (brightness*8));

  //show_Data(ledBar);
}

void show_Data(ledBar* ledBar)
{
  sendData(ledBar, GLB_CMDMODE);
	
  for (unsigned char i = 0; i < 10; i++)
  {
	  // Go forward on __state
      sendData(ledBar, ledBar->state[i]);
  }
  // Two extra empty bits for padding the command to the correct length
  sendData(ledBar, 0x00);
  sendData(ledBar, 0x00);

  latchData(ledBar);
}
