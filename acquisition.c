#include "acquisition.h"
#include "input.h"
#include "adc.h"
#include "timer.h"

void Acquisition_Trigger(uint16_t* dest_vector, uint16_t total_samples){
	Input_Wait();
	ADC12_StartConversion(dest_vector, total_samples);
	ADC12_StopConversion();
}
