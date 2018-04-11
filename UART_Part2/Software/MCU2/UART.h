// UART.h
// Runs on TM4C123

//------------UART_Init------------
// Initialize the UART1 for 9600 baud rate (assuming 80 MHz clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void);

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void);
unsigned char UART_InChar_B(void);

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data);

//------------UART_OutDec------------
// Output 32-bit decimal to UART output
// Input: data is a 32-bit decimal value to be transferred
// Output: none
void UART_OutDec(unsigned long data);

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char str[]);
