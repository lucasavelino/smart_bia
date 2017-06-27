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
#include "leds.h"

#define NUMBER_SAMPLES 200
#define NUMBER_TEST_VALUES 50

service_t services[1];

uint16_t samples[NUMBER_SAMPLES];

float test_values[NUMBER_TEST_VALUES] = {
		543.043681802178,
		574.239102587031,
		619.068022223272,
		585.430066578857,
		592.042784696115,
		577.660429554848,
		571.311499712381,
		587.928054684462,
		559.256128491154,
		559.355067910320,
		546.424095430454,
		603.274119575261,
		607.253096905081,
		592.562182934181,
		589.994586815812,
		587.035240525158,
		575.338870541575,
		581.667225180854,
		570.834386767418,
		550.652344636403,
		598.017513086727,
		597.065427355211,
		569.739124758395,
		577.995909164471,
		581.922694518754,
		570.158234214809,
		591.748044402346,
		539.992187615644,
		590.054344503833,
		606.758661042668,
		571.710035161267,
		559.780524076398,
		568.267202535242,
		569.412328904423,
		595.031709260801,
		568.646235414491,
		603.186759660772,
		562.595873619623,
		585.098114984400,
		550.514409885266,
		580.621724533690,
		597.958880067115,
		560.050497432668,
		589.810738993725,
		586.286630145844,
		578.604213523386,
		580.075941536416,
		599.871545602162,
		565.844774937704,
		587.712298526504
};

int main(void) {
	int32_t i;
    Watchdog_Stop();
    Clock_Init_16MHz();
    AD9850_Init();
    ADC12_Init();
    Leds_Init();
	Input_Init();
	Esp8266_Init();

	AD9850_SetFreq(5000,0);
	while(1) {
		Acquisition_Trigger(samples, NUMBER_SAMPLES);
		Uiot_ClientRegister();
		Uiot_ServiceInit(services, "GetImpedance", FLOAT);
		Uiot_ServiceAddParameter(services,FLOAT,"example_parameter");
		Uiot_ServiceRegister(services,1);
		for (i = 0; i < NUMBER_TEST_VALUES; ++i) {
			Uiot_DataRegister(services[0].service_id, &test_values[i]);
		}
	}
}
