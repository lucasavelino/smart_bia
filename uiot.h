#ifndef UIOT_H_
#define UIOT_H_

#include <stdint.h>

#define NAME_SIZE 50
#define PARAMETERS_SIZE 5

typedef enum {
	INT32_T,
	UINT32_T,
	INT16_T,
	UINT16_T,
	FLOAT,
	DOUBLE
}type_t;

typedef struct {
	type_t type;
	char name[NAME_SIZE];
}parameter_t;

typedef struct {
	char name[NAME_SIZE];
	parameter_t parameters[PARAMETERS_SIZE];
	uint8_t parameters_count;
	type_t return_type;
}service_t;

uint8_t Uiot_ClientRegister(void);
void Uiot_ServiceInit(service_t* service, char* service_name, type_t return_type);
void Uiot_ServiceAddParameter(service_t* service, type_t parameter_type, char* parameter_name);
uint8_t Uiot_ServiceRegister(service_t services[], uint8_t nservices);

#endif /* UIOT_H_ */
