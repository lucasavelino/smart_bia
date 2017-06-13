#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void ADC12_Init();
void ADC12_StartConversion(uint16_t* dest, uint16_t total);
void ADC12_StopConversion();

#endif /* ADC_H_ */
