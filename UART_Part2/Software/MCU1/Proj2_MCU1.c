// Proj2_MCU1.c
// Runs on TM4C123
// Implements UART0 to send and receive data 
// from a serial terminal, and UART1 to send and 
// receive data from a second microcontroller.
// April 2, 2018

// 1. Pre-processor Directives Section
// include header files that contain 
// function prototypes and constant definitions
#include "PLL.h"
#include "UART.h"
#include "SysTick.h"
#include "PWM.h"
#include "tm4c123gh6pm.h"

// 3. Misc. Function Prototypes
void EnableInterrupts(void);
void DisableInterrupts(void);

// 4. Main loop
int main(void){
	unsigned long brightness;// controls LED brightness
  unsigned char r, message;// communication variables between MCUs
	DisableInterrupts();     // disable interrupts while initializing
	PLL_Init();              // PLL w/ 80Mhz bus clk
	SysTick_Init();          // No interrupt systick
	UART_Init();             // UART1 and UART0 with 57600 baud rate, 8-bit word,no pairty bit
	PWM_Init(40000,2);       // Initialize hardware PWM on PF2 for blue LED(initially off)
	EnableInterrupts();      // re-enable interrupts when initializing is finished
	while(1){
		// Part 1
		brightness = UART1_InDec();            // receive POT data from MCU2
		if(brightness != 0x0){                 // ignore null inputs
			Duty(((brightness/4095.0)*40000)-1); // update brightness of LED based on POT
			UART0_OutString("ADC Value: ");      // output to terminal
			UART0_OutDec(brightness);            // adc value
			UART0_OutChar(0x0D);                 // Carriage return	
			UART0_OutChar(0x0A);                 // newline
		}
		SysTick_Wait10ms(1);                
		
		// Part 2
		r = UART0_InChar();           // get command from terminal
		if(r == 'r'){
			UART1_OutChar('r');         // send blink command to MCU2		
		}
		message = UART1_InChar();     // get confrimation message
		if(message == 'y'){
			UART0_OutString("Red LED is now blinking");
			UART0_OutChar(0x0D);        // Carriage return	
			UART0_OutChar(0x0A);        // newline
		}
		else if(message == 'b'){
			UART0_OutString("Red LED is already blinking");
			UART0_OutChar(0x0D);        // Carriage return	
			UART0_OutChar(0x0A);        // newline
		}
		else if(message == 'n'){
      UART0_OutString("Red LED is now off");
			UART0_OutChar(0x0D);        // Carriage return	
			UART0_OutChar(0x0A);        // newline
		}
		else if(message == 'a'){
			UART0_OutString("Red LED is already off");
			UART0_OutChar(0x0D);        // Carriage return	
			UART0_OutChar(0x0A);        // newline
		}
		SysTick_Wait10ms(1);
	}
}
