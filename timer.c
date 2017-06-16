#include "timer.h"
#include <msp430.h>

static inline void Wait_1ms(void);

void Wait_ms(uint32_t time_ms){
	volatile uint32_t wait_time = time_ms;
	while(wait_time--){
		Wait_1ms();
	}
}

static inline void Wait_1ms(void){
	TA1EX0 = 0;
	TA1CCR0 = 33-1;
	TA1CTL = TACLR;
	TA1CTL |= TASSEL__ACLK | MC__UP | TAIE;
	__low_power_mode_0();
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_a1_isr(void){
	volatile uint16_t flags = TA1IV;
	TA1CTL = TACLR | MC__STOP;

	__low_power_mode_off_on_exit();
}
