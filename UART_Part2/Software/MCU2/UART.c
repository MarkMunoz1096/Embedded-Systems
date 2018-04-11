// UART.c
#include "UART.h"
#include "tm4c123gh6pm.h"

//------------UART_Init------------
// Initialize the UART1 for 9600 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1; // activate UART1
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART while initializing 
  UART1_IBRD_R = 86;                    // IBRD = int(80,000,000 / (16 * 57600)) = int(86.80555555)
  UART1_FBRD_R = 52;                    // FBRD = int(0.805555 * 64 + 0.5) = 52
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART1_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART1_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTB_AFSEL_R |= 0x03;           // enable alt funct on PB1-0
  GPIO_PORTB_DEN_R |= 0x03;             // enable digital I/O on PB1-0
                                        // configure PB1-0 as UART1
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&~0x000000FF)+0x00000011;
  GPIO_PORTB_AMSEL_R &= ~0x03;          // disable analog functionality on PB
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
	if((UART1_FR_R&UART_FR_RXFE) == 0){
		return((unsigned char)(UART1_DR_R&0xFF));
	}
	else{
		return 0;
	}
}

// Blocking method of receiving char 
unsigned char UART_InChar_B(void){
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART1_DR_R&0xFF));
}

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}

//------------UART_OutDec------------
// Output 32-bit decimal to serial port. Sends one digit at a time
// because UART world length is 8-bits
// Input: data is a decimal value to be transferred
// Output: none
void UART_OutDec(unsigned long data){
	if(data >= 10){
    UART_OutDec(data/10);
    data %= 10; 
  }
  UART_OutChar(data+'0'); // convert int digit to a char
}

//------------UART_OutString------------
// Outputs string to serial port
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char str[]){
	unsigned int i = 0;
  while(str[i] != 0x0){
    UART_OutChar(str[i]);
    i++;
  }
}

