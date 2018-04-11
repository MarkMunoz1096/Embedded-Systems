// Proj2_MCU2.c
// Runs on TM4C123
// Implements UART1 on MCU2 of project 2 to send communicate with MCU1. 
// Receives analog input from POT, adjusts brightness of LED on MCU1.
// Also receives commands from MCU1 to toggle red LED. 
// April 2, 2018

// 1. Pre-processor Directives Section
// include header files that contain 
// function prototypes and constant definitions
#include "PLL.h"
#include "UART.h"
#include "SysTick.h"
#include "ADC.h"
#include "tm4c123gh6pm.h"

// 2. Misc. Function Prototypes
void EnableInterrupts(void);
void DisableInterrupts(void);
void PortF_Init(void);
unsigned int absDiff(unsigned int a, unsigned int b);

// 3. Global variables
volatile unsigned long count, toggle; 

// 4. Main loop
int main(void){
	unsigned char r = 0;    // command from MCU1(initially not a recognized comand)  
	                        // initial POT conditions
	unsigned long potVal=0, potVal2=0, potSelect=0;
	toggle = 0;             // LED not initially toggling
	DisableInterrupts();    // disable interrupts while initializing
	PLL_Init();             // PLL w/ 80Mhz bus clk
	SysTick_Init();         // 10Hz timer interrupt 
	UART_Init();            // UART1 with 57600 baud rate, 8-bit word,no pairty bit
	Pot_Init();             // Initialize ADC on PE0 for POT
	PortF_Init();           // On-board RED LED and push button 0
	EnableInterrupts();     // re-enable interrupts when initializing is finished
	while(1){
	  // 1)
		if(potSelect == 0){
			potVal=Pot_Read();  // sample POT
			potSelect = 1;      // change variable reading POT
		}
		else{
			potVal2=Pot_Read(); // sample POT
			potSelect = 0;      // change variable reading POT
		}
		if(absDiff(potVal, potVal2) > 5){// only update if pot is turning
			potVal=Pot_Read();  // get most recent POT sample
			UART_OutDec(potVal);// send data to MCU1
			UART_OutChar(0x0D); // Carriage return ends decimal reading 
		}
		else{
			UART_OutChar(0x0D); // Carriage return ends decimal reading 
		}
		SysTick_Wait10ms(1);
		
		// 2)
		r = UART_InChar();    // get command data from MCU1
		if(r=='r'){           // if command recognized
			if(toggle){
				UART_OutChar('b');// already toggling message  
			}
			else{
				toggle = 1;       // start toggling
				UART_OutChar('y');// now toggling message
			}
		}
		SysTick_Wait10ms(1);
	}
}

// 5. Subroutines Section
// Subroutine to initialize onboard red LED and push button0 (PF4)
// Input:  none
// Output: none
void PortF_Init(void){ 
	// GPIO Assignments
	volatile unsigned long delay;     // dummy instruction variable
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // dummy instruction to give setup enough time   
  GPIO_PORTF_LOCK_R  |=  0x4C4F434B;// 2) unlock PortF   
  GPIO_PORTF_CR_R    |=  0x12;      // allow changes to PF4,PF1    
  GPIO_PORTF_AMSEL_R &= ~0x12;      // 3) disable analog function on PF4,PF1
  GPIO_PORTF_PCTL_R  &= ~0x000F00F0;// 4) GPIO clear bit PCTL 
	GPIO_PORTF_DIR_R   &= ~0x10;      //    PF4 is input	
	GPIO_PORTF_DIR_R   |=  0x02;      //    PF1 is output
  GPIO_PORTF_AFSEL_R &= ~0x12;      // 6) no alternate function on PF4,PF1
	GPIO_PORTF_PUR_R   |=  0x10;      //    enable pullup resistors for push button(positive logic)
	GPIO_PORTF_PUR_R   &= ~0x02;      //    disable pullup resistors for Red LED
  GPIO_PORTF_DEN_R   |=  0x12;      // 7) enable digital pins PF4,PF1

	// interrupt assignments
	GPIO_PORTF_IS_R  &= ~0x10;        // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    	  //     PF4 is not both edge active
  GPIO_PORTF_IEV_R |=  0x10;        //     PF4 falling edge event(positive logic)
  GPIO_PORTF_ICR_R  =  0x10;        // (e) clear flag 4 
  GPIO_PORTF_IM_R  |=  0x10;     		// (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      		// (h) enable interrupt 30 in NVIC		
}


// ISR for on-board switch 0(PF4)
// Input:  none
// Output: none
void GPIOPortF_Handler(void){ 
	GPIO_PORTF_ICR_R |= 0x10;     // acknowledge interrupt flag
	if(toggle){
		UART_OutChar('n');          // 'LED now off' signal
		toggle = 0;                 // stop toggleing red LED
	}
	else{
		UART_OutChar('a');          // 'LED already off' signal
	}
	GPIO_PORTF_DATA_R &= ~0x02;   // ensure red LED is off
}	

// Timer interrupt to toggle red LED if toggle flag is high
// Input:  none
// Output: none
void SysTick_Handler(void){
	if(count%10 == 9){            // number of interrupts for 1 second delay
		if(toggle){
			GPIO_PORTF_DATA_R ^= 0x02;// toggle every second
		}
	}
	count ++;
}

// Subroutine to get absolute value of difference between two unsigned ints
// Input:  two unsigned integers a and b
// Output: absolute value of difference between  a and b
unsigned int absDiff(unsigned int a, unsigned int b){
	if (a > b)
		return a - b;
	else
		return b - a;
}

