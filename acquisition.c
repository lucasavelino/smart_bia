#include "acquisition.h"
#include "input.h"
#include "adc.h"
#include "timer.h"
#include "leds.h"

void Acquisition_Trigger(uint16_t* dest_vector, uint16_t total_samples){
	Leds_SetState(RED,0);
	Leds_SetState(GREEN,1);
	Input_Wait();
	Leds_SetState(GREEN,0);
	Leds_SetState(RED,1);
	ADC12_StartConversion(dest_vector, total_samples);
	ADC12_StopConversion();
}
