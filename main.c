#include <msp430.h>
#include "wdt.h"
#include "clock.h"
#include "uart.h"
#include "esp8266.h"
#include "uiot.h"
#include "timer.h"
#include "ad9850.h"
#include "input.h"
#include "adc.h"
#include "acquisition.h"

#define NUMBER_SAMPLES 500

service_t services[1];

uint16_t samples[NUMBER_SAMPLES];

int main(void) {
    Watchdog_Stop();
    Clock_Init_16MHz();
    AD9850_Init();
//	Input_Init();
//	ADC12_Init();
//	Esp8266_Init();

	//P2DIR |= BIT2;
	//P2SEL |= BIT2;


    AD9850_SetFreq(50000,0);

    Acquisition_Trigger(samples, NUMBER_SAMPLES);

//	Uiot_ClientRegister();
//	Uiot_ServiceInit(services, "GetImpedance", FLOAT);
//	Uiot_ServiceAddParameter(services,FLOAT,"example_parameter");
//	Uiot_ServiceRegister(services,1);
	while(1){}
}
