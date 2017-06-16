#include "adc.h"
#include <stdint.h>
#include <msp430.h>

static uint16_t sample_count;
static uint16_t* result_addr;
static uint16_t total_samples;

void ADC12_Init(){
	ADC12CTL0 &= ~ADC12ENC; 	//Disabling conversion to configure ADC12
	REFCTL0 &= ~REFMSTR;		//Reference system controlled by control bits inside the ADC12
	ADC12CTL0 |= ADC12SHT0_0 | ADC12ON | ADC12REFON | ADC12MSC;
								//sampling-and-hold time => 4 ADC12CLK cycles + 13 cycles = 17 cycles
								//ADC12_A on
								//ADC12 reference on
								//Multiple sample-and-conversion. As quickly as possible.
	ADC12CTL0 &= ~ADC12REF2_5V; //ADC12_A reference generator voltage on - 1.5V
	ADC12CTL1 = ADC12SSEL_0 | ADC12CONSEQ_2 | ADC12SHP;
							//ADC12_A clock source => ADCOSC
							//Repeat single channel conversion mode selected
							//SAMPCON from sampling timer -> to enable multiple sample-and-conversion
	ADC12CTL2 |= ADC12RES_2 | ADC12TCOFF;
							//12 bit of resolution
							//Temperature sensor off
    ADC12MCTL0 = ADC12EOS | ADC12SREF_1 | ADC12INCH_0;
    						//End-of-sequence
    						//V(R+) = VREF+ and V(R-) = AVSS
    						//A0 channel selected
    ADC12IE = ADC12IE0;
    						//ADC12 interrupt enable in channel zero
}

void ADC12_StartConversion(uint16_t* dest, uint16_t total){
	sample_count=0;
	result_addr = dest;
	total_samples = total;
	ADC12CTL0 |= ADC12ENC | ADC12SC;
	__low_power_mode_0();
}

void ADC12_StopConversion(){
	ADC12CTL0 &= ~ADC12ENC;
}

#pragma vector=ADC12_VECTOR
__interrupt void Adc12_Isr(void){
	switch(__even_in_range(ADC12IV, 0x24)){
		case 0x00:
			break;
		case 0x02:
			break;
		case 0x04:
			break;
		case 0x06:
			result_addr[sample_count] = ADC12MEM0;
			sample_count++;
			if(sample_count >= total_samples){
				sample_count = 0;
				ADC12IE &= ~ADC12IE0;
				ADC12CTL0 &= ~ADC12ENC;
				__low_power_mode_off_on_exit();
			}
			break;
		case 0x08:
		case 0x0A:
		case 0x0C:
		case 0x0E:
		case 0x10:
		case 0x12:
		case 0x14:
		case 0x16:
		case 0x18:
		case 0x1A:
		case 0x1C:
		case 0x1E:
		case 0x20:
		case 0x22:
		case 0x24:
		default:
			break;
	}
}
