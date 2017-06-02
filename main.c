#include <msp430.h>
#include "wdt.h"
#include "clock.h"
#include "uart.h"
#include "esp8266.h"
#include "uiot.h"
#include "timer.h"
#include "ad9850.h"

service_t services[1];

int main(void) {
    Watchdog_Stop();
    Clock_Init_16MHz();
    AD9850_Init();
    AD9850_SetFreq(50000,0);
	Esp8266_Init();

	Uiot_ClientRegister();
	Uiot_ServiceInit(services, "GetImpedance", FLOAT);
	Uiot_ServiceAddParameter(services,FLOAT,"example_parameter");
	Uiot_ServiceRegister(services,1);
	while(1){}
}
