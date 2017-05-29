#include "ad9850.h"
#include <msp430.h>

#define W_CLK_OUT 		P1OUT
#define W_CLK_BIT	 	BIT4
#define FQ_UD_OUT		P1OUT
#define FQ_UD_BIT		BIT5
#define DATA_OUT		P2OUT
#define DATA_BIT		BIT4
#define DATA_SHIFT_L	(4u)
#define RESET_OUT		P2OUT
#define RESET_BIT		BIT5


void AD9850_Init(void){

    P1SEL &= ~(BIT4|BIT5);
    P2SEL &= ~(BIT4|BIT5);
    P1DIR |= BIT4|BIT5;
    P2DIR |= BIT4|BIT5;

    W_CLK_OUT &= ~W_CLK_BIT;
    FQ_UD_OUT &= ~FQ_UD_BIT;
    DATA_OUT &= ~DATA_BIT;
    RESET_OUT &= ~RESET_BIT;

    RESET_OUT |= RESET_BIT;
	RESET_OUT &= ~RESET_BIT;

    W_CLK_OUT |= W_CLK_BIT;
	W_CLK_OUT &= ~W_CLK_BIT;

	FQ_UD_OUT |= FQ_UD_BIT;
	FQ_UD_OUT &= ~FQ_UD_BIT;

}


void AD9850_SetFreq(double freq, uint8_t phase){
	uint8_t i;
	uint32_t value = (uint32_t) (freq/7.135);
	for (i = 0; i < 32; ++i) {
		if(i < 32){
			DATA_OUT = ((DATA_OUT&(~DATA_BIT)) | (((value>>i)&BIT0) << DATA_SHIFT_L));
		}
		else if(i < 35){
			DATA_OUT &= ~DATA_BIT;
		}
		else{
			DATA_OUT = ((DATA_OUT&(~DATA_BIT)) | (((phase>>(i-35))&BIT0) << DATA_SHIFT_L));
		}
		W_CLK_OUT |= W_CLK_BIT;
		W_CLK_OUT &= ~W_CLK_BIT;
	}
	FQ_UD_OUT |= FQ_UD_BIT;
	FQ_UD_OUT &= ~FQ_UD_BIT;
}
