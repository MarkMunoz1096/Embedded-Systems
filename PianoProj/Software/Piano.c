// Piano.c
// Uses input from a 4x4 keypad matrix to change 
// SysTick timer interrupt frequency
#include "Piano.h"
#include "SysTick.h"
#include "Sound.h"
#include "tm4c123gh6pm.h"

// **************Piano_Init*********************
// Initialize piano key I/O on PD0-3 and PC4-7
// Input: none
// Output: none
void Piano_Init(void){volatile unsigned long delay;       // dummy instruction variable
	SYSCTL_RCGC2_R |= 0x0000000C;                           // Enable clock for PORTD and PORTC
	delay = SYSCTL_RCGC2_R;                                 // Delay
	GPIO_PORTD_CR_R    |= 0x0F;                             // Allow changes for PD0-3
	GPIO_PORTC_CR_R    |= 0xF0;                             // Allow changes for PC4-7
	GPIO_PORTD_AMSEL_R &= ~0x0F;                            // Disable analog function for PD0-3
	GPIO_PORTC_AMSEL_R &= ~0xF0;                            // Disable analog function for PC4-7
	GPIO_PORTD_PCTL_R  &= ~0x0000FFFF;                      // Set PortD as GPIO
	GPIO_PORTC_PCTL_R  &= ~0xFFFF0000;                      // Set PortC as GPIO
	GPIO_PORTD_DIR_R   |= 0x0F;                             // Set PD0-3 output
	GPIO_PORTC_DIR_R   &= ~0xF0;                            // Set PC4-7 input
	GPIO_PORTD_AFSEL_R &= ~0x0F;                            // Disable alternate function for PD0-3
	GPIO_PORTC_AFSEL_R &= ~0xF0;                            // Disable alternate function for PAC4-7
	GPIO_PORTC_PDR_R   |= 0xF0;                             // Enable pulldown resistors for PC4-7(positive logic)
	GPIO_PORTD_DEN_R   |=  0x0F;                            // Enable digital pins for PD0-3 
	GPIO_PORTC_DEN_R   |=  0xF0;                            // Enable digital pins for PC4-7 
}

// **************Read_Key*********************
// Get input from 'piano keys' and take action based on input
// Input: none 
// Output: none
// Connections: {PC4, PC5, PC6, PC7} = {Row1, Row2, Row3, Row4}
//              {PD3, PD2, PD1, PD0} = {Col4, Col3, Col2, Col1}
// Output Pin:{ 0    1    2    3    4    5    6    7
// GPIO:      {PC4, PC5, PC6, PC7, PD3, PD2, PD1, PD0}

void Read_Key(void){
	// set Column 4 output
	GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R&~0x0F)|0x01;
	// check which row is on
	if(GPIO_PORTC_DATA_R&0x10){
		Sound_Tone(RELOAD_349Hz); // 'A' pressed on Col4, Row1 --Output Sine Waveform at 349Hz
		return;
	}
	else if(GPIO_PORTC_DATA_R&0x20){
		GPIO_PORTF_DATA_R = 0x08; // 'B' pressed on Col4, Row2 --Debug LED
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x40){
		GPIO_PORTF_DATA_R = 0x04; // 'C' pressed on Col4, Row3 --Debug LED
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x80){
		GPIO_PORTF_DATA_R = 0x02; // 'D' pressed on Col4, Row4 --Debug LED
		return;
	}	
	
	// set Column 3 output
	GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R&~0x0F)|0x02;
	// check which row is on
	if(GPIO_PORTC_DATA_R&0x10){
		Sound_Tone(RELOAD_330Hz); // '3' pressed on Col3, Row1--Output Sine Waveform at 330Hz
		return;
	}
	else if(GPIO_PORTC_DATA_R&0x20){
		Sound_Tone(RELOAD_494Hz); // '6' pressed on Col3, Row2 --Output Sine Waveform at 494Hz
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x40){
		GPIO_PORTF_DATA_R = 0x04; // '9' pressed on Col3, Row3 --Debug LED
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x80){
		GPIO_PORTF_DATA_R = 0x02; // '#' pressed on Col3, Row4 --Debug LED
		return;
	}	
	
	// set Column 2 output
	GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R&~0x0F)|0x04;
	// check which row is on
	if(GPIO_PORTC_DATA_R&0x10){
		Sound_Tone(RELOAD_294Hz); // '2' pressed on Col2, Row1 --Output Sine Waveform at 294Hz
		return;
	}
	else if(GPIO_PORTC_DATA_R&0x20){
		Sound_Tone(RELOAD_440Hz); // '5' pressed on Col2, Row2 --Output Sine Waveform at 440Hz
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x40){
		GPIO_PORTF_DATA_R = 0x04; // '8' pressed on Col2, Row3 --Debug LED
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x80){
		GPIO_PORTF_DATA_R = 0x02; // '0' pressed on Col2, Row4 --Debug LED
		return;
	}	
	
		// set Column 1 output
	GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R&~0x0F)|0x08;
	
	// check which row is on
	if(GPIO_PORTC_DATA_R&0x10){
		Sound_Tone(RELOAD_262Hz); // '1' pressed on Col1, Row1 --Output Sine Waveform at 262Hz
		return;
	}
	else if(GPIO_PORTC_DATA_R&0x20){
		Sound_Tone(RELOAD_392Hz); // '4' pressed on Col1, Row2 --Output Sine Waveform at 392Hz
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x40){
		GPIO_PORTF_DATA_R = 0x04; // '7' pressed on Col1, Row3 --Debug LED
		return;
	}	
	else if(GPIO_PORTC_DATA_R&0x80){
		GPIO_PORTF_DATA_R = 0x02; // '*' pressed on Col1, Row4 --Debug LED
		return;
	}	

	// none pressed check
	if((GPIO_PORTC_DATA_R&0xF0)==0x00){
		GPIO_PORTF_DATA_R &= ~0x0E;// turn off all debug LEDs
		Sound_Off();               // turn off sound
	} 
}

