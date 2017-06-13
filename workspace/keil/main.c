#include "main.h"
#include "Display.h"

struct I2C_Module
{
  I2C_HandleTypeDef   instance;
  uint16_t            sdaPin;
  GPIO_TypeDef*       sdaPort;
  uint16_t            sclPin;
  GPIO_TypeDef*       sclPort;
};
void LED_Init_GPIO();
void MX_WWDG_Init(WWDG_HandleTypeDef* hwwdg);
void I2C_ClearBusyFlagErratum(struct I2C_Module* i2c);
void MX_I2C1_Init(I2C_HandleTypeDef* hi2c1);
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
	__HAL_RCC_GPIOD_CLK_ENABLE();
//	ledBar bar;
//	bar.pinClock= GPIO_PIN_11;
	//bar.pinData = GPIO_PIN_12;
	//LED_Bar_init(&bar);
	/*
	WWDG_InitTypeDef def;
	WWDG_HandleTypeDef  wwdog;
	I2C_HandleTypeDef h1;
		struct segment_display display;
	display.i2c=&h1;
	struct I2C_Module modul;
	modul.instance=*display.i2c;
	modul.sclPort=GPIOB;
	modul.sclPin=GPIO_PIN_6;
	modul.sdaPort=GPIOB;
	modul.sdaPin=GPIO_PIN_7;	*/
//	MX_I2C1_Init(&h1);
//	LED_Init_GPIO();
	//setup();
//	clearDisplay(&display);
//	setBrightness(&display,15,MILLIS_TO_TICK(5000));
//	blinkRate(&display,2,MILLIS_TO_TICK(5000));
	int counter = 0;
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
	display.segments[7] = GPIO_PIN_7; //SEG Point
	display.segments[8] = GPIO_PIN_2; //SEG Double Point

	init_display(&display);
	uint32_t i= 0;
	uint32_t last = HAL_GetTick();
	bool doublePointLight = false;
	while(1) {
		writeDigit(&display,0,i,false);
		writeDigit(&display,1,(i+1)%10,false);
		writeDigit(&display,2,(i+2)%10,true);
		writeDigit(&display,3,(i+3)%10,true);
		
		writeDoublePoint(&display,doublePointLight);
			
		if(HAL_GetTick()-last>1000) {
			i=(i+1)%10;
			last = HAL_GetTick();
			doublePointLight= !doublePointLight;
		}
		HAL_Delay(MILLIS_TO_TICK(10));
	}
}

 
/* WWDG init function */
static void MX_WWDG_Init(WWDG_HandleTypeDef* hwwdg)
{

  __HAL_RCC_WWDG_CLK_ENABLE();
  hwwdg->Instance = WWDG;
  hwwdg->Init.Prescaler = WWDG_PRESCALER_2;
  hwwdg->Init.Counter = 0x7f;
  hwwdg->Init.Window = 0x7f ;


	//TIMOEOUT = 
  hwwdg->Init.EWIMode = WWDG_EWI_DISABLE;
	uint32_t wwdgClock = HAL_RCC_GetPCLK1Freq() / (4096* hwwdg->Init.Prescaler);
	uint32_t wwdgTimeout = 1000* hwwdg->Init.Counter/wwdgClock;
	uint32_t max = 1000* (hwwdg->Init.Counter-0x40) /wwdgClock;
		/*
	(+) The WWDG counter input clock is derived from the APB clock divided 
        by a programmable prescaler.
    (+) WWDG clock (Hz) = PCLK1 / (4096 * Prescaler)
    (+) WWDG timeout (mS) = 1000 * Counter / WWDG clock
	//    counter = timeout/1000*wwdgclock
    (+) WWDG Counter refresh is allowed between the following limits :
        (++) min time (mS) = 1000 * (Counter _ Window) / WWDG clock
        (++) max time (mS) = 1000 * (Counter _ 0x40) / WWDG clock*/
	
	
  if (HAL_WWDG_Init(hwwdg) != HAL_OK)
  {
    Error_Handler();
  }

}
/* I2C1 init function */
static void MX_I2C1_Init(I2C_HandleTypeDef* hi2c1)
{
  /* RCC Configuration */
  /*I2C Peripheral clock enable */
	
  hi2c1->Instance = I2C1;
  hi2c1->Init.ClockSpeed = 100000; //Standard mode
	
  hi2c1->Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1->Init.OwnAddress1 = 0x71;
  hi2c1->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1->Init.OwnAddress2 = 0x72;
  hi2c1->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(hi2c1) != HAL_OK)
  {
    Error_Handler();
		
  }
}
/*
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {

  
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //  __HAL_RCC_I2C1_CLK_ENABLE();
	//	HAL_I2C_EnableListen_IT(hi2c);
  }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{

  if(hi2c->Instance==I2C1)
  {

    __HAL_RCC_I2C1_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

  }
}*/
void LED_Init_GPIO() {
	GPIO_InitTypeDef LED_Init;	
	
	LED_Init.Mode=GPIO_MODE_OUTPUT_PP;
	LED_Init.Pin= GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_11;
	LED_Init.Pull=GPIO_NOPULL;
	LED_Init.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD,&LED_Init);
}
