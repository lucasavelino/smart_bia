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

#define NUMBER_SAMPLES 200

service_t services[1];

uint16_t samples[NUMBER_SAMPLES];

int main(void) {
    Watchdog_Stop();
    Clock_Init_16MHz();
    AD9850_Init();
    ADC12_Init();
	Input_Init();
	Esp8266_Init();


    AD9850_SetFreq(5000,0);

    Acquisition_Trigger(samples, NUMBER_SAMPLES);

	Uiot_ClientRegister();
	Uiot_ServiceInit(services, "GetImpedance", FLOAT);
	Uiot_ServiceAddParameter(services,FLOAT,"example_parameter");
	Uiot_ServiceRegister(services,1);
	while(1){}
}
