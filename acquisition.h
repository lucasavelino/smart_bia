#ifndef ACQUISITION_H_
#define ACQUISITION_H_
#include "stdint.h"

void Acquisition_Trigger(uint16_t* dest_vector, uint16_t total_samples);

#endif /* ACQUISITION_H_ */
