#include "main.h"

static void Error_Handler(){
	while(1);
}
void SystemClock_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
 __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	
	
    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
		/****************************************
	 *SystemFrequency/1000      1000us    
	 *SystemFrequency/10000     100us
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/TICK_RATE);
	
  /**Configure the Systick */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


int main(void) {
	HAL_Init();

	SystemClock_Config();
	struct display display;
	display.port=GPIOD;
	display.digitPins[0] =GPIO_PIN_15; //DISPLAY 1
	display.digitPins[1] =GPIO_PIN_13; //DISPLAY 2
	display.digitPins[2] =GPIO_PIN_11; //DISPLAY 3
	display.digitPins[3] =GPIO_PIN_9; //DISPLAY 4
	display.digitPins[4] =GPIO_PIN_0; //DISPLAY Double Point


	display.segments[0] = GPIO_PIN_10; //SEG A
	display.segments[1] = GPIO_PIN_12; //SEG B

	display.segments[2] = GPIO_PIN_14; //SEG C

	display.segments[3] = GPIO_PIN_8; //SEG D
	display.segments[4] = GPIO_PIN_3; //SEG E
	display.segments[5] = GPIO_PIN_5; //SEG F
	display.segments[6] = GPIO_PIN_1; //SEG G
	display.segments[7] = GPIO_PIN_7; //SEG PINT
	init_display(&display);
	uint32_t i= 0;
	while(1) {
		writeDigit(&display,0,i,false);
		writeDigit(&display,1,(i+1)%10,false);
		writeDigit(&display,2,(i+2)%10,true);
		writeDigit(&display,3,(i+3)%10,true);
		
		for(uint32_t c = 0;c<10000;c++) {}
	}
}
