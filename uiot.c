#include "uiot.h"
#include "esp8266.h"
#include <stdio.h>
#include <string.h>

#define REQUEST_BUFFER_SIZE 512
#define RESPONSE_BUFFER_SIZE 1024

#define TOKEN_SIZE 50

#define SERVER_IP "164.41.222.112"
#define SERVER_PORT "80"

#define POST_HEADER_FORMAT  "POST %s HTTP/1.1\r\n"\
							"Host: %s\r\n"\
							"Accept: application/json\r\n"\
							"Content-Length: %ld\r\n"\
							"Content-Type: application/json\r\n"\
							"User-Agent: msp430iot/0.1\r\n\r\n"

#define BASE_URI "homol.redes.unb.br"
#define CLIENT_REGISTER_URI "/uiot-raise/client/register/"
#define CLIENT_LIST_URI "/uiot-raise/client/list/"
#define SERVICE_REGISTER_URI "/uiot-raise/service/register?type=true"
#define SERVICE_LIST_URI "/uiot-raise/service/list/"
#define DATA_REGISTER_URI "/uiot-raise/data/register/"
#define DATA_LIST_URI "/uiot-raise/data/list/"

#define CLIENT_REGISTER_BODY "{ "\
								 "\"name\": \"MSP 430\", "\
								 "\"chipset\": \"MSP430F5529\", "\
								 "\"mac\": \"5E:CF:7F:87:39:85\", "\
								 "\"serial\": \"ESP8266\", "\
								 "\"processor\": \"MSP430\", "\
								 "\"channel\": \"Wi-Fi\", "\
								 "\"client_time\": 2019915671, "\
								 "\"tag\": [] "\
							 "}"

#define CODE_KEY "\"code\""
#define TOKEN_KEY "\"tokenId\""

#define SERVICES_INIT "{ \"services\": [ "
#define SERVICE_FORMAT "{ \"name\": \"%s\", "\
					           "\"parameters\": { "
#define PARAMETER_FORMAT "\"%s\": \"%s\""
#define COMMA_SPACE ", "
#define SPACE " "
#define CLOSE_BRACES "}"
#define SPACE_CLOSE_BRACKETS " ]"

#define MESSAGE_FOOTER_FORMAT  	"\"tokenId\": \"%s\", "\
	    						"\"client_time\": 2019915672, "\
								"\"tag\": [] "\
								"}"

#define TYPE_AS_STR(t) (t == INT32_T ? "int32_t" : \
					   (t == UINT32_T ? "uint32_t" : \
					   (t == INT16_T ? "int16_t" : \
					   (t == UINT16_T ? "uint16_t" : \
					   (t == FLOAT ? "float" : \
					   (t == DOUBLE ? "double" : "" ))))))

static char request_buffer[REQUEST_BUFFER_SIZE];
static char response_buffer[RESPONSE_BUFFER_SIZE];
static int16_t response_buffer_count = 0;

static uint16_t response_code;
static char token[TOKEN_SIZE];

static uint8_t Json_FilterInt(char* key, uint16_t key_size, uint16_t* value);
static uint8_t Json_FilterStr(char* key, uint16_t key_size, char* value);
static void Uiot_SendRequestGetResponse(uint16_t request_size);

static void Uiot_SendRequestGetResponse(uint16_t request_size){
	Esp8266_Connect(SERVER_IP,SERVER_PORT);
	Esp8266_Write(request_buffer,request_size);
	response_buffer_count = Esp8266_GetResponse(response_buffer,RESPONSE_BUFFER_SIZE);
	if(response_buffer_count >= RESPONSE_BUFFER_SIZE){
		response_buffer[RESPONSE_BUFFER_SIZE-1] = 0;
	}
	else{
		response_buffer[response_buffer_count] = 0;
	}
}

uint8_t Uiot_ClientRegister(void){
	uint32_t body_size = sizeof(CLIENT_REGISTER_BODY);
	uint16_t request_size = snprintf(request_buffer,REQUEST_BUFFER_SIZE,
			POST_HEADER_FORMAT,CLIENT_REGISTER_URI,BASE_URI,body_size);
	request_size += snprintf(&request_buffer[request_size],REQUEST_BUFFER_SIZE-request_size,
			CLIENT_REGISTER_BODY);
	Uiot_SendRequestGetResponse(request_size);
	Json_FilterInt(CODE_KEY,sizeof(CODE_KEY)-1,&response_code);
	if(response_code != 200){
		return 0;
	}
	Json_FilterStr(TOKEN_KEY,sizeof(TOKEN_KEY)-1,token);
	return 1;
}


void Uiot_ServiceInit(service_t* service, char* service_name, type_t return_type){
	strncpy(service->name, service_name, NAME_SIZE);
	service->return_type = return_type;
	service->parameters_count = 0;
}

void Uiot_ServiceAddParameter(service_t* service, type_t parameter_type, char* parameter_name){
	strncpy(service->parameters[service->parameters_count].name, parameter_name, strlen(parameter_name));
	service->parameters[service->parameters_count].type = parameter_type;
	service->parameters_count++;
}

uint8_t Uiot_ServiceRegister(service_t services[], uint8_t nservices){
	uint8_t i;
	uint16_t request_size = snprintf(request_buffer,REQUEST_BUFFER_SIZE,
			POST_HEADER_FORMAT,SERVICE_REGISTER_URI,BASE_URI,999999);
	uint16_t body_size = request_size;
	strncpy(&request_buffer[request_size],SERVICES_INIT,sizeof(SERVICES_INIT));
	request_size += sizeof(SERVICES_INIT);
	for (i = 0; i < nservices; ++i) {
		request_size += snprintf(&request_buffer[request_size-1],REQUEST_BUFFER_SIZE,SERVICE_FORMAT,services[i].name);
		uint8_t j;
		for (j = 0; j < services[i].parameters_count; ++j) {
			request_size += snprintf(&request_buffer[request_size-1],
					REQUEST_BUFFER_SIZE, PARAMETER_FORMAT,
					services[i].parameters[j].name, TYPE_AS_STR(services[i].parameters[j].type));
			if(j < services[i].parameters_count-1){
				strncpy(&request_buffer[request_size-1],COMMA_SPACE, sizeof(COMMA_SPACE));
				request_size += sizeof(COMMA_SPACE)-1;
			}
			else{
				strncpy(&request_buffer[request_size-1],SPACE, sizeof(SPACE));
				request_size += sizeof(SPACE)-1;
			}
		}
		strncpy(&request_buffer[request_size-1],CLOSE_BRACES, sizeof(CLOSE_BRACES));
		request_size += sizeof(CLOSE_BRACES)-1;
		if(i < nservices-1){
			strncpy(&request_buffer[request_size-1],COMMA_SPACE, sizeof(COMMA_SPACE));
			request_size += sizeof(COMMA_SPACE)-1;
		}
		else{
			strncpy(&request_buffer[request_size-1],SPACE_CLOSE_BRACKETS COMMA_SPACE, sizeof(SPACE_CLOSE_BRACKETS COMMA_SPACE));
			request_size += sizeof(SPACE_CLOSE_BRACKETS COMMA_SPACE)-1;
		}
	}
	request_size += snprintf(&request_buffer[request_size-1],REQUEST_BUFFER_SIZE,MESSAGE_FOOTER_FORMAT,token);
	char *pch, aux_str[7];
	body_size = request_size - body_size;
	snprintf(aux_str, 7, "%6d", body_size);
	pch = strstr(request_buffer,"999999\r\n");
	memcpy(pch,aux_str,6);
	Uiot_SendRequestGetResponse(request_size);
	Json_FilterInt(CODE_KEY,sizeof(CODE_KEY)-1,&response_code);
	if(response_code != 200){
		return 0;
	}
	return 1;
}

static uint8_t Json_FilterInt(char* key, uint16_t key_size, uint16_t* value){
	char* foundkey;
	foundkey = strstr(response_buffer,key);
	if(foundkey != 0){
		foundkey = foundkey+key_size;
		while(foundkey < (response_buffer+RESPONSE_BUFFER_SIZE)){
			if(*foundkey < '0' || *foundkey > '9'){
				foundkey++;
			}
			else{
				sscanf((const char *)foundkey,"%d",value);
				return 1;
			}
		}
	}
	return 0;
}

static uint8_t Json_FilterStr(char* key, uint16_t key_size, char* value){
	char* foundkey;
	foundkey = strstr(response_buffer,key);
	if(foundkey != 0){
		foundkey = foundkey+key_size;
		while(foundkey < (response_buffer+RESPONSE_BUFFER_SIZE)){
			if(*foundkey != '"'){
				foundkey++;
			}
			else{
				foundkey++;
				sscanf(foundkey,"%[^\"]s",value);
				return 1;
			}
		}
	}
	return 0;
}
