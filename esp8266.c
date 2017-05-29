#include "uart.h"
#include "esp8266.h"
#include "timer.h"
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define SSID "CASA9"
#define PASSWORD "4lm1rl1m"

#define COMMAND_BUFFER_SIZE 50
#define MAX_ATTEMPTS 10

#define AT_RST_RESPONSE "OK"
#define AT_RST "AT+RST\r\n"
#define AT_CWMODE_RESPONSE "OK"
#define AT_CWMODE_1 "AT+CWMODE=1\r\n"
#define AT_CWJAP_RESPONSE "WIFI CONNECTED\r\n"
#define AT_CWJAP "AT+CWJAP=\"%s\",\"%s\"\r\n"
#define AT_CWLAP_RESPONSE "OK"
#define AT_CWLAP "AT+CWLAP\r\n"
#define AT_CIFSR_RESPONSE "OK"
#define AT_CIFSR "AT+CIFSR\r\n"
#define AT_CIPSTART_RESPONSE "OK"
#define AT_CIPSTART "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n"
#define AT_CIPSEND_RESPONSE "OK"
#define AT_CIPSEND "AT+CIPSEND=%d\r\n"
#define AT_CIPMODE_RESPONSE "OK"
#define AT_CIPMODE "AT+CIPMODE=%d\r\n"
#define AT_RESPONSE "OK"
#define AT "AT\r\n"
#define AT_GMR_RESPONSE "version:"
#define AT_GMR "AT+GMR\r\n"
#define AT_CIPMUX_RESPONSE "OK"
#define AT_CIPMUX "AT+CIPMUX=%d\r\n"
#define AT_CIPAPMAC_RESPONSE "OK"
#define AT_CIPAPMAC "AT+CIPAPMAC?\r\n"
#define RECEIVE_FILTER "+IPD,"


static char command_buffer[COMMAND_BUFFER_SIZE];

static uint8_t ESP8266_Write_Cmd_Wait_Response(const char* command_format, uint16_t wait_time,
		char* response, uint16_t response_size, ...);
static uint8_t ESP8266_Reset();
static uint8_t ESP8266_SetWifiClientMode(void);
static uint8_t ESP8266_JoinAccessPoint(const char* ssid, const char* password);
static uint8_t ESP8266_ListAccessPoints(void);
static uint8_t ESP8266_GetIPAddress(void);
static uint8_t ESP8266_SetDataTransmissionMode(uint8_t mode);
static uint8_t ESP8266_Test(void);
static uint8_t ESP8266_CheckCurrentVersion(void);
static uint8_t ESP8266_SetConnectionMux(uint8_t enable);
static uint8_t ESP8266_GetMacAddress(void);

void Esp8266_Init(void){
	Uart_Init(F115200_0);
	P2SEL &= ~BIT7;
	P2DIR |= BIT7;
	P2OUT |= BIT7;
	__enable_interrupt();

	ESP8266_Reset();

	ESP8266_SetWifiClientMode();

	ESP8266_JoinAccessPoint(SSID,PASSWORD);

	ESP8266_SetDataTransmissionMode(0);
}

uint8_t Esp8266_Connect(char* ip_addr, char* port){
	return ESP8266_Write_Cmd_Wait_Response(AT_CIPSTART, 8000, AT_CIPSTART_RESPONSE,
			sizeof(AT_CIPSTART_RESPONSE)-1, ip_addr, port);
}

uint8_t Esp8266_Write(char* packet, uint16_t packet_size){
	uint16_t command_size = snprintf(command_buffer, COMMAND_BUFFER_SIZE, AT_CIPSEND, packet_size);
	uint16_t attempt = MAX_ATTEMPTS;
	while(attempt){
		Uart_Write(command_buffer, command_size);
		Wait_ms(5);
		Uart_Write(packet,packet_size);
		Wait_ms(8000);
		if(Uart_Search_Read(AT_CIPSEND_RESPONSE, sizeof(AT_CIPSEND_RESPONSE)-1) != NOT_FOUND){
			return 1;
		}
		attempt--;
	}
	return 0;
}

uint16_t Esp8266_GetResponse(char* buffer, uint16_t buffer_size){
	volatile int16_t pos;
	Wait_ms(8000);
	pos = Uart_Search_Read(RECEIVE_FILTER, sizeof(RECEIVE_FILTER)-1);
	if(pos != NOT_FOUND){
		return Uart_Read(buffer,pos,buffer_size);
	}
	return 0;
}

static uint8_t ESP8266_Write_Cmd_Wait_Response(const char* command_format, uint16_t wait_time,
												char* response, uint16_t response_size, ...){
	va_list argptr;
	va_start(argptr, response_size);
	uint16_t command_size = vsnprintf(command_buffer, COMMAND_BUFFER_SIZE, command_format, argptr);
	va_end(argptr);
	uint16_t attempt = MAX_ATTEMPTS;
	while(attempt){
		Uart_Write(command_buffer, command_size);
		Wait_ms(wait_time);
		if(Uart_Search_Read(response, response_size) != NOT_FOUND){
			return 1;
		}
		attempt--;
	}
	return 0;
}

static uint8_t ESP8266_GetMacAddress(void){
	return ESP8266_Write_Cmd_Wait_Response(AT_CIPAPMAC,5000,AT_CIPAPMAC_RESPONSE,sizeof(AT_CIPAPMAC_RESPONSE)-1);
}

static uint8_t ESP8266_SetConnectionMux(uint8_t enable){
	return ESP8266_Write_Cmd_Wait_Response(AT_CIPMUX,5000,AT_CIPMUX_RESPONSE,sizeof(AT_CIPMUX_RESPONSE)-1);
}

static uint8_t ESP8266_Test(void){
	return ESP8266_Write_Cmd_Wait_Response(AT, 8000, AT_RESPONSE, sizeof(AT_RESPONSE)-1);
}

static uint8_t ESP8266_CheckCurrentVersion(void){
	return ESP8266_Write_Cmd_Wait_Response(AT_GMR, 8000, AT_GMR_RESPONSE, sizeof(AT_GMR_RESPONSE)-1);
}

static uint8_t ESP8266_SetWifiClientMode(void){
	return ESP8266_Write_Cmd_Wait_Response(AT_CWMODE_1, 5000, AT_CWMODE_RESPONSE, sizeof(AT_CWMODE_RESPONSE)-1);
}

static uint8_t ESP8266_JoinAccessPoint(const char* ssid, const char* password){
	return ESP8266_Write_Cmd_Wait_Response(AT_CWJAP, 8000, AT_CWJAP_RESPONSE, sizeof(AT_CWJAP_RESPONSE)-1,
			ssid, password);
}

static uint8_t ESP8266_Reset(){
	P2OUT &= ~BIT7;
	Wait_ms(10);
	P2OUT |= BIT7;
	return ESP8266_Write_Cmd_Wait_Response(AT_RST, 5000, AT_RST_RESPONSE, sizeof(AT_RST_RESPONSE)-1);
}

static uint8_t ESP8266_ListAccessPoints(void){
	return ESP8266_Write_Cmd_Wait_Response(AT_CWLAP, 10000, AT_CWLAP_RESPONSE, sizeof(AT_CWLAP_RESPONSE)-1);
}

uint8_t ESP8266_GetIPAddress(void){
	return ESP8266_Write_Cmd_Wait_Response(AT_CIFSR, 5000, AT_CIFSR_RESPONSE, sizeof(AT_CIFSR_RESPONSE)-1);
}

static uint8_t ESP8266_SetDataTransmissionMode(uint8_t mode){
	return ESP8266_Write_Cmd_Wait_Response(AT_CIPMODE,5000,AT_CIPMODE_RESPONSE,sizeof(AT_CIPMODE_RESPONSE)-1,mode);
}
