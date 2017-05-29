#ifndef ESP8266_H_
#define ESP8266_H_

#include <stdint.h>

// P3.4 (RX)       -> TX (1)
// P3.3 (TX)       -> RX (8)
// P2.7            -> RESET (3)
// 3.3V            -> VCC (4)
// 3.3V            -> GPIO0 (7)
// 3.3V            -> GPIO2 (6)
// 3.3V -> 10KOhms -> CH_PD (2)
// GND             -> GND (5)

void Esp8266_Init(void);
uint8_t Esp8266_Write(char* packet, uint16_t packet_size);
uint8_t Esp8266_Connect(char* ip_addr, char* port);
uint16_t Esp8266_GetResponse(char* buffer, uint16_t buffer_size);

#endif /* ESP8266_H_ */
