#ifndef QUEUES_H
#define QUEUES_H

#include <FreeRTOS.h>
#include <queue.h>
#include "debug_if.h"
#include <string.h>
#include <stdio.h>
#include "uart_term.h"

#define QUEUESIZE 20
#define MQTT_TOPIC_SIZE 40
#define MQTT_PAYLOAD_SIZE 100
typedef enum{
    APP_MQTT_PUBLISH,
    APP_MQTT_RECEIVE,
    APP_MQTT_CONNACK
} eventType;

typedef struct {
    eventType event;
    char topic[MQTT_TOPIC_SIZE];
    char payload[MQTT_PAYLOAD_SIZE];
} mqttMsg;

typedef struct {
    double timestamp;
    int sequenceNum;
    char topic[MQTT_TOPIC_SIZE];
    char payload[MQTT_PAYLOAD_SIZE];
} unpackedMsg;

typedef struct {
} msgTriggerRGBSwitch;

int init_queue();

int sendToMqttQueueIsr(mqttMsg*);
int sendToMqttQueue(mqttMsg*);
int receiveFromMqttQueue(mqttMsg*);

int sendToGenQueueIsr(unpackedMsg*);
int sendToGenQueue(unpackedMsg*);
int receiveFromGenQueue(unpackedMsg*);

int sendMsgToQueueTriggerRGBSwitch(msgTriggerRGBSwitch*);
int receiveMsgFromQueueTriggerRGBSwitch(msgTriggerRGBSwitch*);

#endif
