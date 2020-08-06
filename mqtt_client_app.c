#include "mqtt_client_app.h"

char* allTopics[ALL_TOPIC_COUNT] = {SUB_TOPIC_0, SUB_TOPIC_1, SUB_TOPIC_2, SUB_TOPIC_3, PUB_TOPIC_0};

void MQTT_EventCallback(int32_t event){
    mqttMsg connackMsg = {APP_MQTT_CONNACK, "", ""};
    if (event==MQTT_EVENT_CONNACK)
        sendToMqttQueueIsr(&connackMsg);
}

void BrokerCB(char* topic, char* payload){
    mqttMsg recMsg;
    recMsg.event=APP_MQTT_RECEIVE;
    strcpy(recMsg.topic, topic);
    strcpy(recMsg.payload, payload);
    sendToMqttQueueIsr(&recMsg);
}


void mainThread(void * args){

    SPI_init();
    if(ti_net_SlNet_initConfig())
        LOG_ERROR("Failed to initialize SlNetSock\n\r");

    if(WifiInit())
        errorHalt("Error initializing WiFi");
    char ClientId[13] = "";
    SetClientIdNamefromMacAddress(ClientId);
    mqttClientParams.clientID = ClientId;

    if(MQTT_IF_Init(mqttInitParams))
        errorHalt("Error initializing MQTT interface");

    #pragma diag_suppress=551 //mqttClientHandle used before initialized
    MQTTClient_Handle mqttClientHandle;
    int32_t ret=0;
    int i;
    for(i=0; i<NUM_SUB_TOPICS; i++)
        ret |= MQTT_IF_Subscribe(mqttClientHandle, allTopics[i], QOS, BrokerCB);
    if(ret)
        errorHalt("Error subscribing");

    mqttClientHandle = MQTT_IF_Connect(mqttClientParams, mqttConnParams, MQTT_EventCallback);
    if(mqttClientHandle < 0)
        errorHalt("Error connecting");

    initTimerUS();
    init_pubTimer();
    init_statsTimer();

    mqttMsg recMsg;
    unpackedMsg unpMsg;

    do
        receiveFromMqttQueue(&recMsg);
    while (recMsg.event!=APP_MQTT_CONNACK);

    while(1){
        receiveFromMqttQueue(&recMsg);
        if(recMsg.event == APP_MQTT_PUBLISH){
            MQTT_IF_Publish(mqttClientHandle, recMsg.topic, recMsg.payload, QOS);
        }
        else if(recMsg.event == APP_MQTT_RECEIVE){
            unpMsg.statsCmd=RECEIVED;
            strcpy(unpMsg.topic, recMsg.topic);
            json_unpack(&unpMsg, recMsg.payload);
            if(strcmp(recMsg.topic, SUB_TOPIC_3)==0)
                sendToGenQueue(&unpMsg);
            sendToStatsQueue(&unpMsg);
        }
    }
}



