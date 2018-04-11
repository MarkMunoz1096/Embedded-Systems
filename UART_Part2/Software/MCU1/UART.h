// UART.h
// Runs on TM4C123

// U0Rx (VCP receive)  connected to PA0
// U0Tx (VCP transmit) connected to PA1
// U1Rx (VCP receive)  connected to PB0
// U1Tx (VCP transmit) connected to PB1

//------------UART_Init------------
// Initialize UART0 and UART1 for 57600 baud rate (assuming 80 MHz clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void);

//------------UARTx_InChar------------
// Wait for new serial port input(both blocking and non blocking methods)
// Input: none
// Output: ASCII code for key typed
unsigned char UART0_InChar(void);
unsigned char UART0_InChar_B(void);
unsigned char UART1_InChar(void);
unsigned char UART1_InChar_B(void);

//------------UARTx_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART0_OutChar(unsigned char data);
void UART1_OutChar(unsigned char data);


//------------UARTx_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART0_OutString(char str[]);
void UART1_OutString(char str[]);

//------------UARTx_InDec------------
// Returns 32-bit decimal value 
// Input: none
// Output: 32-bit decimal value
unsigned long UART0_InDec(void);
unsigned long UART1_InDec(void);

//------------UARTx_OutDec------------
// Output 32-bit decimal to UART output
// Input: data is a 32-bit decimal value to be transferred
// Output: none
void UART0_OutDec(unsigned long data);
void UART1_OutDec(unsigned long data);
