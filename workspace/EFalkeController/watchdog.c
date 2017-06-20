 
 #include "watchdog.h"
 
static void Error_Handler() {
	while(1) {
	}
}
/* WWDG init function */
void WWDG_Init(WWDG_HandleTypeDef* hwwdg, uint32_t clockFreq)
{

 __HAL_RCC_WWDG_CLK_ENABLE();
	
	
	// wwdgClock = clockFreq / (4096* hwwdg->Init.Prescaler);
	// wwdgTimeout = 1000* hwwdg->Init.Counter/wwdgClock;
	// max Time = (1000* (hwwdg->Init.Counter-0x40) /wwdgClock) ms;
	// min Time= (1000 * (Counter _ Window) / WWDG clock) ms

  hwwdg->Instance = WWDG;
  hwwdg->Init.Prescaler = WWDG_PRESCALER_2;
  hwwdg->Init.Counter = 0x7f;
  hwwdg->Init.Window = 0x7f ;


  hwwdg->Init.EWIMode = WWDG_EWI_DISABLE;
		/*
	(+) The WWDG counter input clock is derived from the APB clock divided 
        by a programmable prescaler.
    (+) WWDG clock (Hz) = PCLK1 / (4096 * Prescaler)
    (+) WWDG timeout (mS) = 1000 * Counter / WWDG clock
	//    counter = timeout/1000*wwdgclock*/
  if (HAL_WWDG_Init(hwwdg) != HAL_OK)
  {
    Error_Handler();
  }
}
