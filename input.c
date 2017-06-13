#include "input.h"
#include "timer.h"
#include <msp430.h>

static inline void Switch1_Init(){
	P2DIR &= ~BIT1;		// P2.1 -> Input Switch
	P2REN |= BIT1;		// Pull-up/down resistor enabled
	P2OUT |= BIT1;		// Pull-down resistor
}

void Input_Init(){
	Switch1_Init();
}

static inline void Switch1_Wait(){
	// debounce logic
//	do{
//		P2IES |= BIT1;			// falling edge interrupt
//		P2IE |=  BIT1;			// Enables interrupt
//		__low_power_mode_0();	// Wait for interrupt
//		Wait_ms(5);				// Wait 5ms more and check state
//	}while(P2IN&BIT1);			// Go back if button is not pressed
//	do{
//		P2IES &= ~BIT1;			// rising edge interrupt
//		P2IE |=  BIT1;			// Enables interrupt
//		__low_power_mode_0();	// Wait for interrupt
//		Wait_ms(5);				// Wait 5ms more and check state
//	}while(!(P2IN&BIT1));		// Go back if button is pressed
	do{
		while(P2IN&BIT1);
		Wait_ms(5);
	}while(P2IN&BIT1);
	do{
		while(!(P2IN&BIT1));
		Wait_ms(5);
	}while(!(P2IN&BIT1));
}

void Input_Wait(){
	Switch1_Wait();
}

//#pragma vector=PORT2_VECTOR
//__interrupt void Port2_Isr(void){
//	switch(__even_in_range(P2IV, 0x10)){
//		case 0x00:
//			break;
//		case 0x02:
//			P2IE &= ~BIT1;
//			P2IFG &= ~BIT1;
//			__low_power_mode_off_on_exit();
//			break;
//		case 0x04:
//		case 0x06:
//		case 0x08:
//		case 0x0A:
//		case 0x0C:
//		case 0x0E:
//		case 0x10:
//			break;
//	}
//}
