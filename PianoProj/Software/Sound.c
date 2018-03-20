// Sound.c
// Use the SysTick timer to request interrupts at a user given frequency.
#include "Sound.h"
#include "DAC.h"
#include "tm4c123gh6pm.h"

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt frequency
// Output: none
void Sound_Tone(unsigned long frequency){
	NVIC_ST_RELOAD_R = frequency; // given user frequency
  NVIC_ST_CTRL_R = 0x07;        // enable SysTick Timer w/ interrupt
}


// **************Sound_Off*********************
// stop outputing to DAC
// Input:  none
// Output: none
void Sound_Off(void){
	NVIC_ST_CTRL_R = 5;          // disable SysTick interrupt, leave SysTick counting
	DAC_Out(0x00);               // set DAC outputs to 0
}
