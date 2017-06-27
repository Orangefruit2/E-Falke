/*----------------------------------------------------------------------------
 * Name:    CanDemo.c
 * Purpose: CAN example for STM32F407
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f4xx.h>
#include "Serial.h"
#include "CAN.h"
#include "LED.h"


unsigned int val_Tx = 0, val_Rx = 0;              /* Globals used for display */

volatile uint32_t msTicks;                        /* counts 1ms timeTicks     */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

#ifndef __NO_SYSTICK
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
#else
  for (curTicks = 0; curTicks < (dlyTicks * 0x1000); curTicks++) __NOP();
#endif
}


/*----------------------------------------------------------------------------
  display transmit and receive values
 *---------------------------------------------------------------------------*/
void val_display (void) {

  LED_Out (val_Rx);                               /* display RX val on LEDs  */
}


/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void can_Init (void) {

  CAN_setup (1);                                  /* setup CAN Controller #1  */
#ifndef __TEST
  CAN_setup (2);                                  /* setup CAN Controller #2  */
#endif
  CAN_wrFilter (1, 33, STANDARD_FORMAT);          /* Enable reception of msgs */

#ifdef __TEST
  //CAN_testmode(1, CAN_BTR_SILM | CAN_BTR_LBKM);   // Loopback, Silent Mode (self-test)
	CAN_testmode(1, CAN_BTR_LBKM);
#endif

  CAN_start (1);                                  /* start CAN Controller #1  */
#ifndef __TEST
  CAN_start (2);                                  /* start CAN Controller #2  */
#endif

  CAN_waitReady (1);                              /* wait til tx mbx is empty */
#ifndef __TEST
  CAN_waitReady (2);                              /* wait til tx mbx is empty */
#endif
}



/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)  {
  int i;

  LED_Init ();                                    /* initialize the LEDs      */
	
#ifndef __TEST
	printf("Loopback mode disabled:\n");
#else
	printf("Loopback mode enabled:\n");
#endif
	
  SystemCoreClockUpdate();                        /* Get Core Clock Frequency */
#ifndef __NO_SYSTICK
  SysTick_Config(SystemCoreClock /1000);         /* SysTick 1 msec irq       */
#endif
  can_Init ();                                    /* initialize CAN interface */

  CAN_TxMsg[1].id = 33;                           /* initialize msg to send   */
  for (i = 0; i < 8; i++) CAN_TxMsg[0].data[i] = 0;
  CAN_TxMsg[1].len = 1;
  CAN_TxMsg[1].format = STANDARD_FORMAT;
	CAN_TxMsg[1].type = DATA_FRAME;

  while (1) {

    val_Tx = (val_Tx + 1) % 15;

#ifndef __TEST
    if (CAN_TxRdy[1]) {                           /* tx msg on CAN Ctrl #2    */
      CAN_TxRdy[1] = 0;

      CAN_TxMsg[1].data[0] = val_Tx;              /* data[0] = ADC value      */
			for (i = 1; i < 8; i++) CAN_TxMsg[1].data[i] = 0x77;
      CAN_wrMsg (2, &CAN_TxMsg[1]);               /* transmit message         */
    }
#else
    if (CAN_TxRdy[0]) {                           /* tx msg on CAN Ctrl #2    */
      CAN_TxRdy[0] = 0;

      CAN_TxMsg[1].data[0] = val_Tx;              /* data[0] = ADC value      */
      CAN_wrMsg (1, &CAN_TxMsg[1]);               /* transmit message         */
    }
#endif

    Delay (10000);                                   /* delay for 10ms           */

    //if (CAN_RxRdy[0]) {                           /* rx msg on CAN Ctrl #1    */
    //  CAN_RxRdy[0] = 0;

      val_Rx = CAN_RxMsg[0].data[0];
    //}

    val_display ();                               /* display TX and RX values */
    Delay (500);                                  /* delay for 500ms          */

  }
}
