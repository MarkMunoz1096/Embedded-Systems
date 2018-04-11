// UART.c
#include "UART.h"
#include "tm4c123gh6pm.h"

//------------UART_Init------------
// Initialize UART0 and UART 1 for 57600 baud rate (with 80 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
	// UART0
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART while initializing 
  UART0_IBRD_R = 86;                    // IBRD = int(80,000,000 / (16 * 57600)) = int(86.80555555)
  UART0_FBRD_R = 52;                    // FBRD = int(0.805555 * 64 + 0.5) = 52
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA
	
	// UART1
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
                                        // configure PB1-0 as UART
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&~0x000000FF)+0x00000011;
  GPIO_PORTB_AMSEL_R &= ~0x03;          // disable analog functionality on PB
}

//------------UARTx_InChar------------
// Wait for new serial port input 
// Input: none
// Output: ASCII code for data received
unsigned char UART0_InChar(void){
  if((UART0_FR_R&UART_FR_RXFE) == 0){
		return((unsigned char)(UART0_DR_R&0xFF));
	}
	else{
		return 0;
	}
}
unsigned char UART1_InChar(void){
  if((UART1_FR_R&UART_FR_RXFE) == 0){
		return((unsigned char)(UART1_DR_R&0xFF));
	}
	else{
		return 0;
	}
}

// Blocking method of receiving char
unsigned char UART0_InChar_B(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}

unsigned char UART1_InChar_B(void){
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART1_DR_R&0xFF));
}

//------------UARTx_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART0_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}
void UART1_OutChar(unsigned char data){
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}


//------------UARTx_OutString------------
// Outputs through UART
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART0_OutString(char str[]){
	unsigned int i = 0;
  while(str[i] != 0x0){
    UART0_OutChar(str[i]);
    i++;
  }
}
void UART1_OutString(char str[]){
	unsigned int i = 0;
  while(str[i] != 0x0){
    UART1_OutChar(str[i]);
    i++;
  }
}

//------------UARTx_InDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long UART1_InDec(void){
unsigned long number=0, length=0;
char character;
  character = UART1_InChar();
  while(character != 0x0D){ // accepts until CR is received
    if((character>='0') && (character<='9')) {
      number = 10*number+(character-'0');  // construct the number
      length++;
    }
    character = UART1_InChar();
  }
  return number;
}

unsigned long UART0_InDec(void){
unsigned long number=0, length=0;
char character;
  character = UART0_InChar();
  while(character != 0x0D){ // accepts until CR is received
    if((character>='0') && (character<='9')) {
      number = 10*number+(character-'0');  // construct the number
      length++;
    }
		character = UART0_InChar();
  }
  return number;
}

//------------UARTx_OutDec------------
// Output 32-bit decimal to serial port. Sends one digit at a time
// because UART world length is 8-bits
// Input: data is a decimal value to be transferred
// Output: none
void UART0_OutDec(unsigned long data){
	if(data >= 10){
    UART0_OutDec(data/10);
    data %= 10; 
  }
  UART0_OutChar(data+'0'); // convert int digit to a char
}

void UART1_OutDec(unsigned long data){
	if(data >= 10){
    UART1_OutDec(data/10);
    data %= 10; 
  }
  UART1_OutChar(data+'0'); // convert int digit to a char
}

