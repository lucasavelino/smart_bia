#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

enum pwm_frequency_t {
	F100, F1000
};

void Wait_ms(uint32_t time_ms);
void PWM_Init(enum pwm_frequency_t freq, uint8_t duty_cicle);

#endif /* TIMER_H_ */
