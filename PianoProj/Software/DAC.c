// DAC.c
// Configures 6-bit DAC on PA2-PA7

#include "DAC.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 6-bit DAC on GPIO_PORTA
// Input: none
// Output: none
void DAC_Init(void){volatile unsigned long delay;         // dummy instruction variable
	SYSCTL_RCGC2_R |= 0x00000001;                           // Enable clock to PORTA
	delay = SYSCTL_RCGC2_R;                                 // Delay
	GPIO_PORTA_LOCK_R = 0x4C4F434B;                         // Unlock PortA
	GPIO_PORTA_CR_R    |= 0xFC;                             // Allow changes for PA2-7
	GPIO_PORTA_AMSEL_R &= ~0xFC;                            // Disable analog function for PA2-7
	GPIO_PORTA_PCTL_R  &= ~0xFFFFFF00;                      // GPIO clear bit PCTL to set as GPIO
	GPIO_PORTA_DIR_R   |= 0xFC;                             // Set PA2-7 output
	GPIO_PORTA_AFSEL_R &= ~0xFC;                            // Disable alternate function for PA2-7
	GPIO_PORTA_PUR_R   &= ~0xFC;                            // Disable pullup resistors for PA2-7
	GPIO_PORTA_DEN_R   |=  0xFC;                            // Enable digital pins for PA2-7
	}

//***************PortF_Init********************
// Initialize On Board Push Button to Change Piano Modes
// Input: none
// Output: none
void PortF_Init(void){ 
	// GPIO assignments
	volatile unsigned long delay;     // dummy instruction variable
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // dummy instruction to give setup enough time   
  GPIO_PORTF_LOCK_R  |=  0x4C4F434B;// 2) unlock PortF PF0  
  GPIO_PORTF_CR_R    |=  0x1E;      // allow changes to PF4-PF1     
  GPIO_PORTF_AMSEL_R &= ~0x1E;      // 3) disable analog function
  GPIO_PORTF_PCTL_R  &= ~0x000FFFF0;// 4) GPIO clear bit PCTL  
	GPIO_PORTF_DIR_R   &= ~0x10;      // 5) PF4 input
	GPIO_PORTF_DIR_R   |=  0x0E;      //    PF3-PF1 is input
  GPIO_PORTF_AFSEL_R &= ~0x1E;      // 6) no alternate function on PF4-PF1
	GPIO_PORTF_PUR_R   |=  0x10;      //    enable pullup resistors for push button
	GPIO_PORTF_PUR_R   &= ~0x0E;      //    disable pullup resistors for push buttons
  GPIO_PORTF_DEN_R   |=  0x1E;      // 7) enable digital pins PF4-PF1 
	
	// interrupt assignments
	GPIO_PORTF_IS_R  &= ~0x10;        // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    	  //     PF4 is not both edge active
  GPIO_PORTF_IEV_R |= 0x10;         //     PF4 falling edge event(negative logic)
  GPIO_PORTF_ICR_R  =  0x10;        // (e) clear flag 4
  GPIO_PORTF_IM_R  |=  0x10;     		// (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      		// (h) enable interrupt 30 in NVIC	     
}
	
// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Output: none
void DAC_Out(unsigned long data){
	GPIO_PORTA_DATA_R = (GPIO_PORTA_DATA_R&~0xFC)+(data<<2);// Assign PA2-PA7 with the 6-bit value data
}
