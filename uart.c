#include "uart.h"
#include <msp430.h>

static char uart_buffer[UART_BUFFER_SIZE];
static uint16_t uart_buffer_count = 0;

static inline void Uart_Clear(void);

void Uart_Init(enum frequency_t baud_rate){
	uint8_t ucbr_0, ucbr_1, ucbrf, ucbrs, ucos;
	switch (baud_rate) {
		case F9600_0:
			ucos = 0;
			ucbr_0 = 211;
			ucbr_1 = 6;
			ucbrs = 5;
			ucbrf = 0;
			break;
		case F56700_0:
			ucos = 0;
			ucbr_0 = 39;
			ucbr_1 = 1;
			ucbrs = 7;
			ucbrf = 0;
			break;
		case F74880_0:
			ucos = 0;
			ucbr_0 = 224;
			ucbr_1 = 0;
			ucbrs = 0;
			ucbrf = 0;
			break;
		case F74880_1:
			ucos = 1;
			ucbr_0 = 14;
			ucbr_1 = 0;
			ucbrs = 0;
			ucbrf = 0;
			break;
		case F75000_0:
			ucos = 0;
			ucbr_0 = 223;
			ucbr_1 = 0;
			ucbrs = 6;
			ucbrf = 0;
			break;
		case F75000_1:
			ucos = 1;
			ucbr_0 = 13;
			ucbr_1 = 0;
			ucbrs = 2;
			ucbrf = 12;
			break;
		case F115200_0:
			ucos = 0;
			ucbr_0 = 145;
			ucbr_1 = 0;
			ucbrs = 5;
			ucbrf = 0;
			break;
		case F115200_1:
			ucos = 1;
			ucbr_0 = 9;
			ucbr_1 = 0;
			ucbrs = 0;
			ucbrf = 2;
			break;
		default:
			ucos = 0;
			ucbr_0 = 0;
			ucbr_1 = 0;
			ucbrs = 0;
			ucbrf = 0;
			break;
	}
	UCA0CTL1 = UCSWRST;
	UCA0CTL0 = 0x00;
	UCA0CTL1 |= UCSSEL__SMCLK;
	UCA0BR0 = ucbr_0;
	UCA0BR1 = ucbr_1;
	UCA0MCTL = (ucbrf << 4) | (ucbrs << 1) | ucos;
	UCA0CTL1 &= ~UCSWRST;
	P3SEL |= BIT3|BIT4;
	UCA0IE = UCRXIE;
}

void Uart_Write_Byte(char data){
	while(!(UCA0IFG&UCTXIFG));
	UCA0TXBUF = data;
}


void Uart_Write(char *data, uint16_t length){
	uint16_t i;
	Uart_Clear();
	for (i = 0; i < length; ++i) {
		Uart_Write_Byte(data[i]);
	}
}

#pragma vector=USCI_A0_VECTOR
__interrupt void usci_a0_isr(void){
	if(uart_buffer_count < UART_BUFFER_SIZE){
		uart_buffer[uart_buffer_count] = UCA0RXBUF;
		uart_buffer_count++;
	}
	else{
		volatile char lost_byte = UCA0RXBUF;
	}
}

uint16_t Uart_Read(char* buf, uint16_t pos, uint16_t length){
	uint16_t i=0;
	if(uart_buffer_count > 0){
		while(i < length && ((i+pos) < uart_buffer_count)){
			buf[i] = uart_buffer[i+pos];
			i++;
		}
	}
	return i;
}

int16_t Uart_Search_Read(const char* str, uint16_t str_size){
	uint16_t i = 0,j = 0;
	while(i < str_size && j < uart_buffer_count){
		if(str[i] == uart_buffer[j]){
			i++;
			j++;
			if(i == str_size){
				return j;
			}
		}
		else if(i > 0){
			i = 0;
		}
		else{
			j++;
		}
	}
	return NOT_FOUND;
}

static inline void Uart_Clear(void){
	__disable_interrupt();
	uart_buffer_count = 0;
	__enable_interrupt();
}
