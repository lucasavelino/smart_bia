#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

enum LED {
	RED,
	GREEN
};

void Leds_Init(void);
void Leds_SetState(enum LED color, uint8_t state);

#endif /* LEDS_H_ */
