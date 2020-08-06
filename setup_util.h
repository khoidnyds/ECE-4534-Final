#ifndef SETUP_UTIL_H_
#define SETUP_UTIL_H_

#include "mqtt_client_app.h"


extern MQTTClient_Will mqttWillParams;
extern MQTT_IF_ClientParams_t mqttClientParams;
extern MQTT_IF_InitParams_t mqttInitParams;
extern MQTTClient_ConnParams mqttConnParams;


int WifiInit();
int32_t SetClientIdNamefromMacAddress(char* ClientId);


#endif /* SETUP_UTIL_H_ */
