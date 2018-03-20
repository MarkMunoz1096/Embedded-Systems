// Proj1.c
// CECS 447 Project #1: Piano Project
// Has 4 Operating modes:
// Mode 1: DAC outputs a triangle waveform at 262Hz
// Mode 2: DAC outputs a sine waveform at 262Hz
// Mode 3: DAC outputs a square waveform at 262Hz
// Mode 4: Read input from a 4x4 keypad and output a different 
//         note based on which key is pressed. All notes are
//         sinewave outputs with different frequencies. 

#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "SysTick.h"
#include "DAC.h"
#include "Sound.h"
#include "Piano.h"

// Global variables used to generate triangle, square, and sine waveforms
int mode, clk_cnt, DACOUT;

// 128 discrete samples taken from f(x) = 1.65sin(pi*x/64)+1.65, 
// where x = array index, and array value = PA2-7 Output that corresponds to f(x)
const int sinewave[128]=   {32, 34, 35, 36, 38, 39, 41, 43, 45, 46, 47, 49, 50, 52, 53, 54, 
														55, 56, 57, 58, 59, 60, 60, 61, 62, 62, 63, 63, 63, 63, 63, 63, 
														63, 63, 63, 63, 63, 62, 62, 61, 61, 60, 60, 59, 58, 57, 56, 55,
														54, 52, 51, 50, 49, 48, 46, 45, 43, 42, 40, 39, 37, 35, 34, 32,
														30, 28, 27, 25, 24, 22, 21, 19, 18, 16, 15, 14, 13, 11, 10,  9, 
														 8,  7,  6,  5,  4,  4,  3,  3,  2,  2,  1,  1,  0,  0,  0,  0,
														 0,  0,  1,  1,  1,  2,  2,  3,  3,  4,  5,  6,  7,  8,  9, 10, 
														11, 12, 14, 15, 17, 18, 19, 20, 21, 23, 24, 26, 28, 29, 30, 31};

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

int main(void){ 
	DisableInterrupts();        // Disable while initializing 
	SysTick_Init();             // Systick Interrupt Initially set to 262Hz
	DAC_Init();                 // 6-bit DAC on PA2-PA7
	Piano_Init();               // 8-bit Keypad(4x4 Matrix) on PD0-3 and PC4-7
	PortF_Init();               // External Interrupt Switch to Change Piano Modes
	mode=0;clk_cnt=0; DACOUT=0; // mode, clk_cnt used for generating 128 data points per period of a waveform, and initial DAC output
	EnableInterrupts();         // enable after all initialization are done
	
  while(1){

		// Triangle Waveform Generation
		if(mode%4==0){
			Sound_Tone(RELOAD_262Hz);                   // Output Triangle Wave at 262Hz
			if(clk_cnt<=63){
				DACOUT = clk_cnt;                         // count up triangle wave for half a period
			}
			else if(clk_cnt>63 && clk_cnt<=127){
				DACOUT = -1*(clk_cnt-127);                // count down triangle wave for half a period
			}
		}
		
		// Sine waveform Generation
		else if(mode%4==1){
			Sound_Tone(RELOAD_262Hz);                    // Output Sine Wave at 262Hz
			DACOUT = sinewave[clk_cnt];                  // DAC=1.65sin(pi*x/64)+1.65
		}
		
		// Square Waveform Generation
		else if(mode%4==2){
			Sound_Tone(RELOAD_262Hz);                    // Output Square Wave at 262Hz
			if(clk_cnt<=63){                             // 0-63 clock ticks high(50% duty cycle)
				DACOUT = 0x3F;                             // set waveform high
			}
			else if(clk_cnt>63&&clk_cnt<=127){           // 64-127 clock ticks low(50% duty cycle)
				DACOUT = 0x00;                             // set waveform low
			}
		}			
		
		// Get Input from Keypad and Determine Output Frequency
		else if(mode%4==3){
			Read_Key();                                  // Change Output Frequency based on key pressed
			DACOUT = sinewave[clk_cnt];                  // Output is a Sine Waveform
		}
	}         
}

void SysTick_Handler(){
	DAC_Out(DACOUT);                                // Update PA2-PA7
	clk_cnt++;                                      // increment clock count(used for generating waveforms)
	if(clk_cnt>=128){
		clk_cnt=0;                                    // 0<=clk_cnt<=127
	}
}


// ISR That Changes Operating Mode of System
// Modes[i] = {262Hz Triangle Wave, 262Hz Sine Wave, 262Hz Square Wave, Variable Frequency Sine Wave}
void GPIOPortF_Handler(void){
	SysTick_Wait10ms(2);             // debounce delay
	GPIO_PORTF_ICR_R |= 0x10;        // acknowledge interrupt flag
	mode ++;                         // change operating mode
}

