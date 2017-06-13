/**Configure the Systick interrupt time (HOW oftenen interupt)
    */
		/****************************************
	 *1000      1000us    
	 *10000     100us
	 *100000    10us        *
	 *1000000   1us         *
	 *****************************************/

#define TICK_RATE 1000
#define MILLIS_TO_TICK(X) ((uint32_t) (X* TICK_RATE/1000.0))
#define MICROS_TO_TICK(X) ((uint32_t) (X* TICK_RATE/1000000.0))