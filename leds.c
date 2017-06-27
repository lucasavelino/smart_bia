#include <msp430.h>
#include "leds.h"


void Leds_Init(void){
	P1OUT &= ~BIT0;
	P4OUT &= ~BIT7;
	P1SEL &= ~BIT0;
	P4SEL &= ~BIT7;
	P1DIR |= BIT0;
	P4DIR |= BIT7;
}

void Leds_SetState(enum LED color, uint8_t state){
	uint8_t bit;
	uint8_t *reg_out;
	switch (color) {
		case GREEN:
			reg_out = &P4OUT;
			bit = 7;
			break;
		case RED:
		default:
			reg_out = &P1OUT;
			bit = 0;
			break;
	}
	if(state > 1){
		state = 0;
	}
	*reg_out = ((*reg_out) & (~(1 << bit))) | (state << bit);
}
