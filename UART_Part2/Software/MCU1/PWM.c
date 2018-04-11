// PWM.c
// Runs on TM4C123
// Initializes hardware PWM on PF2(blue LED)
#include "tm4c123gh6pm.h"
#include "PWM.h"

#define SYSCTL_RCC_USEPWMDIV    0x00100000  // Enable PWM Clock Divisor
#define SYSCTL_RCC_PWMDIV_M     0x000E0000  // PWM Unit Clock Divisor

// Output on PF2/M1PWM6
void PWM_Init(unsigned long period, unsigned long duty){
  volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x02;             // 1) activate PWM1
  SYSCTL_RCGCGPIO_R |= 0x20;            // 2) activate port F
  delay = SYSCTL_RCGCGPIO_R;            // allow time to finish activating
  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; // 3) use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //    clear PWM divider field
  SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_2;  //    configure for /2 divider(40MHz)
  PWM1_3_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM1_3_GENA_R = 0x0C8;                // PF2 goes low on LOAD and high on CMPA down
  PWM1_3_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM1_3_CMPA_R = duty - 1;             // 6) count value when output rises
  PWM1_3_CTL_R |= 0x00000001;           // 7) start PWM1
  PWM1_ENABLE_R |= 0x00000040;          // enable PF2/M1PWM6
	
	// LED Init
	SYSCTL_RCGC2_R    |= 0x00000020;      // 1) F clock
  delay = SYSCTL_RCGC2_R;               // dummy instruction to give setup enough time   
  GPIO_PORTF_CR_R   |= 0x04;            // allow changes to PF2   
  GPIO_PORTF_AMSEL_R&= ~0x04;           // 3) disable analog function
  GPIO_PORTF_PCTL_R &= ~0x00000F00;     // 4) GPIO clear bit PCTL  
	GPIO_PORTF_PCTL_R |= 0x000000500;     // 4) set PF2 as PWM
  GPIO_PORTF_DIR_R  |= 0x04;            // 5) PF2 output   
	GPIO_PORTF_AFSEL_R|= 0x04;            // 6) enable alt funct on PF2
  GPIO_PORTF_PUR_R  &= ~0x04;           //    disable pullup resistors for LEDs    
  GPIO_PORTF_DEN_R  |= 0x04;            // 7) enable digital pins PF4 andPF1 
}

// change duty cycle of PF2
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void Duty(unsigned int duty){
  PWM1_3_CMPA_R = duty - 1;             // 6) count value when output rises
}
