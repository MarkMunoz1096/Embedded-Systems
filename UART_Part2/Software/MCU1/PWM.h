// PWM.h
// Runs on TM4C123
// Hardware PWM on PF2(blue LED)

// period is 16-bit number of PWM clock cycles in one period (3<=period)
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM_Init(unsigned long period, unsigned long duty);

// change duty cycle of PF2
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void Duty(unsigned duty);
