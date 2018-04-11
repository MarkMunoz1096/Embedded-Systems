// ADCSWTrigger.c
// Runs on LM4F120/TM4C123
// Initializes PE0 as ADC for a POT
#include "ADC.h"
#include "tm4c123gh6pm.h"

//------------Pot_Init------------
// Initialize PE0 as ADC using ADC0, Sequencer 3(1 sample)
// Input: none
// Output: none
void Pot_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x01;      // 2) make PE0 input
  GPIO_PORTE_AFSEL_R |= 0x01;     // 3) enable alternate function on PE0
  GPIO_PORTE_DEN_R &= ~0x01;      // 4) disable digital I/O on PE0
  GPIO_PORTE_AMSEL_R |= 0x01;     // 5) enable analog function on PE0
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3 while init
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+3; // 11) channel Ain3 (PE0)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}

//------------Pot_Read------------
// Read value from POT
// Input: none
// Output: Converted digital value from ADC input
unsigned long Pot_Read(void){  
	unsigned long result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

