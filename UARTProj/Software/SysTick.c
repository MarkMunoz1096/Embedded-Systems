	// SysTick.c
	#include "tm4c123gh6pm.h"
	
	// Initialize SysTick with busy wait running at bus clock.
	void SysTick_Init(void){
	  NVIC_ST_CTRL_R = 0;                                        // disable SysTick during setup
	  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;                       // max reload value
	  NVIC_ST_CURRENT_R = 0;                                     // any write to current clears it
	  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC; // enable SysTick timer w/o interrupt
	}
	
	// Time delay using busy wait.
	void SysTick_Wait(unsigned long delay){
	  volatile unsigned long elapsedTime;
	  unsigned long startTime = NVIC_ST_CURRENT_R;
	  do{
		elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	  }
	  while(elapsedTime <= delay);
	}
	// Time delay using busy wait.
	// This assumes 80 MHz system clock.
	void SysTick_Wait10ms(unsigned long delay){
	  unsigned long i;
	  for(i=0; i<delay; i++){
		SysTick_Wait(8000000);  // wait 10ms (assumes 80 MHz clock)
	  }
	}
