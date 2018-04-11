// Proj2.c
// Runs on TM4C123
// Implements UART to receive commands
// from a serial terminal that control LEDs
// March 19, 2018

// 1. Pre-processor Directives Section
// include header files that contain 
// function prototypes and constant definitions
#include "PLL.h"
#include "UART.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"

// 3. Misc. Function Prototypes
void EnableInterrupts(void);
void DisableInterrupts(void);
void PortF_Init(void);

// 4. Main loop
int main(void){
	unsigned int mode;  // controls LEDs being toggled
	DisableInterrupts();// disable interrupts while initializing
	PLL_Init();         // PLL w/ 80Mhz bus clk
	SysTick_Init();     // used for debounce delays
	UART_Init();        // UART with 9600 baud rate, 8-bit word,no pairty bit
	PortF_Init();       // On-board RGB LED and 2 push buttons
	EnableInterrupts(); // re-enable interrupts when initializing is finished
	while(1){
		mode = UART_InChar();           // receive updated mdoe(blocking method)
		if(mode == 0x67){               // ASCII 'g' is received
			GPIO_PORTF_DATA_R ^= 0x08;  // toggle green LED
			if(GPIO_PORTF_DATA_R&0x08){ // output confirmation message
				UART_OutString("Green LED is on"); 
			}
			else{
				UART_OutString("Green LED is off"); 
			}
			UART_OutChar(0x0A);         // newline
			UART_OutChar(0x0D);         // Carriage return
		}
		
		else if (mode == 0x72){         // ASCII 'r' is received
			GPIO_PORTF_DATA_R ^= 0x02;  // toggle red LED
			if(GPIO_PORTF_DATA_R&0x02){ // output confirmation message
				UART_OutString("Red LED is on"); 
			}
			else{
				UART_OutString("Red LED is off"); 
			}
			UART_OutChar(0x0A);         // newline
			UART_OutChar(0x0D);         // Carriage return
		}
		
		else if (mode == 0x62){         // ASCII 'b' is received
			GPIO_PORTF_DATA_R ^= 0x04;  // toggle blue LED
			if(GPIO_PORTF_DATA_R&0x04){ // output confirmation message
				UART_OutString("Blue LED is on"); 
			}
			else{
				UART_OutString("Blue LED is off"); 
			}
			UART_OutChar(0x0A);         // newline
			UART_OutChar(0x0D);         // Carriage return
		}
	}
}

// 5. Subroutines Section
// Subroutine to initialize onboard LEDs and push buttons
// Input:  none
// Output: none
void PortF_Init(void){ 
  volatile unsigned long delay;     // dummy instruction variable
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // dummy instruction to give setup enough time   
  GPIO_PORTF_LOCK_R  |=  0x4C4F434B;// 2) unlock PortF   
  GPIO_PORTF_CR_R    |=  0x1F;      // allow changes to PF4-PF0     
  GPIO_PORTF_AMSEL_R &= ~0x1F;      // 3) disable analog function on PF4-PF0
  GPIO_PORTF_PCTL_R  &= ~0x000FFFFF;// 4) GPIO clear bit PCTL 
  GPIO_PORTF_DIR_R   &= ~0x11;      //    PF4-PF0 is input	
  GPIO_PORTF_DIR_R   |=  0x0E;      //    PF3-PF1 is output
  GPIO_PORTF_AFSEL_R &= ~0x1F;      // 6) no alternate function on PF4-PF0
  GPIO_PORTF_PUR_R   |=  0x11;      //    enable pullup resistors for push buttons(positive logic)
  GPIO_PORTF_PUR_R   &= ~0x0E;      //    disable pullup resistors for LEDs
  GPIO_PORTF_DEN_R   |=  0x1F;      // 7) enable digital pins PF4-PF0

  // interrupt assignments
  GPIO_PORTF_IS_R  &= ~0x11;        // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;        //     PF4,PF0 is not both edge active
  GPIO_PORTF_IEV_R |=  0x11;        //     PF4,PF0 falling edge event(positive logic)
  GPIO_PORTF_ICR_R  =  0x11;        // (e) clear flag 4 and 0
  GPIO_PORTF_IM_R  |=  0x11;     	// (f) arm interrupt on PF4, PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      	// (h) enable interrupt 30 in NVIC		
}


// ISR for on-board switches
// Input:  none
// Output: none
void GPIOPortF_Handler(void){ 
	SysTick_Wait10ms(2);          // debounce delay
	if(GPIO_PORTF_RIS_R&0x10){    // SW1 pressed
		GPIO_PORTF_ICR_R |= 0x10; // acknowledge interrupt flag
								  // output confirmation message
		if(GPIO_PORTF_DATA_R&0x0E){
			UART_OutString("ALL LEDs are already on");       
		}
		else{
			GPIO_PORTF_DATA_R |= 0x0E;// turn on all LEDs
			UART_OutString("ALL LEDs have been turned on");       
		}
		UART_OutChar(0x0A);       // newline
		UART_OutChar(0x0D);       // Carriage return

	}	
	if(GPIO_PORTF_RIS_R&0x01){    // SW2 pressed
		GPIO_PORTF_ICR_R |= 0x01; // acknowledge interrupt flag
								  // output confirmation message
		if(!(GPIO_PORTF_DATA_R&0x0E)){
			UART_OutString("ALL LEDs are already off");       
		}
		else{
			GPIO_PORTF_DATA_R &= ~0x0E;// turn off all LEDs
			UART_OutString("ALL LEDs have been turned off");       
		}		
		UART_OutChar(0x0A);       // newline
		UART_OutChar(0x0D);       // Carriage return
	}
}
