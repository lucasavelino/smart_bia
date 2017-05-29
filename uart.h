#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#define UART_BUFFER_SIZE 1024

#define NOT_FOUND -1

enum frequency_t{
	F9600_0, F74880_0, F74880_1, F56700_0, F75000_0, F75000_1, F115200_0, F115200_1
};

void Clock_Init_16MHz(void);
void Uart_Init(enum frequency_t baud_rate);
void Uart_Write_Byte(char data);
void Uart_Write(char *data, uint16_t length);
uint16_t Uart_Read(char* buf, uint16_t pos, uint16_t length);
int16_t Uart_Search_Read(const char* str, uint16_t str_size);

#endif /* UART_H_ */
