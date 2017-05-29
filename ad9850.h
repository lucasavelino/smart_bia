#ifndef AD9850_H_
#define AD9850_H_

#include <stdint.h>


// VCC(3.3V) -> VCC(1)
// GND       -> GND (6)
// ********* -> Saida (9)
// P2.5      -> RESET (5)
// P2.4      -> DATA (4)
// P1.5      -> FQ_UD (3)
// P1.4      -> W_CLK (2)

void AD9850_Init(void);
void AD9850_SetFreq(double freq, uint8_t phase);

#endif /* AD9850_H_ */
