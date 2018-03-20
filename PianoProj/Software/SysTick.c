	// SysTick.c
	// Initializes SysTick, provides function for waiting 10ms at 16Mhz system clk
  #include "SysTick.h"

	// Initialize SysTick with interrupt running at user given frequency
	// Inputs: none
	// Outputs: none
	void SysTick_Init(){
	  NVIC_ST_CTRL_R = 0;                                        // disable SysTick during setup
	  NVIC_ST_RELOAD_R = RELOAD_262Hz;                           // timer interrupt at user provided frequency
	  NVIC_ST_CURRENT_R = 0;                                     // any write to current clears it
	  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN; // enable SysTick timer w/ interrupt
	}
	
	// Time delay using busy wait.
	// Input # of clock ticks to wait
	void SysTick_Wait(unsigned long delay){
	  volatile unsigned long elapsedTime;
	  unsigned long startTime = NVIC_ST_CURRENT_R;
	  do{
		elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	  }
	  while(elapsedTime <= delay);
	}
	// 10ms delay using busy wait.
	// This assumes 16 MHz system clock.
	// Inputs: # of 10ms delays
	// Outputs: none
	void SysTick_Wait10ms(unsigned long delay){
	  unsigned long i;
	  for(i=0; i<delay; i++){
		SysTick_Wait(1600000);  // wait 10ms (assumes 16 MHz clock)
	  }
	}
