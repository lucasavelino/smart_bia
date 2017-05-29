#include "clock.h"
#include <msp430.h>

void Clock_Init_16MHz(void){
	UCSCTL0 = 0x00;
	UCSCTL1 = DCORSEL_5;
	UCSCTL2 = FLLD__8 | 63;
	UCSCTL3 = 0x00;
	UCSCTL4 = SELA__XT1CLK |
			  SELS__DCOCLK |
			  SELM__DCOCLK;
	UCSCTL5 = 0x00;

	__delay_cycles(500000);
}
