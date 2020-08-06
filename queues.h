#ifndef QUEUES_H
#define QUEUES_H

#include <FreeRTOS.h>
#include <queue.h>
#include "debug_if.h"
#include <string.h>
#include <stdio.h>
#include "uart_term.h"

#define QUEUESIZE 2
#define MQTT_TOPIC_SIZE 40
#define MQTT_PAYLOAD_SIZE 400
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

typedef enum { PUBLISHED, RECEIVED, PUSH } statisticsCommand;
typedef enum { GENERAL, STATS } messageType;
typedef struct {
    messageType msgType;
    double timestamp;
    int sequenceNum;
    char topic[MQTT_TOPIC_SIZE];
    char payload[MQTT_PAYLOAD_SIZE];
    statisticsCommand statsCmd;
    int numPub;
    int numRec;
    int numDrop;
} unpackedMsg;

typedef struct {
    int triggerUS;
} msgTriggerUS;


int init_queue();

int sendToMqttQueueIsr(mqttMsg*);
int sendToMqttQueue(mqttMsg*);
int receiveFromMqttQueue(mqttMsg*);

int sendToStatsQueueIsr(unpackedMsg*);
int sendToStatsQueue(unpackedMsg*);
int receiveFromStatsQueue(unpackedMsg*);

int sendToGenQueueIsr(unpackedMsg*);
int sendToGenQueue(unpackedMsg*);
int receiveFromGenQueue(unpackedMsg*);


int sendMsgToQueueTriggerUS(msgTriggerUS* outMsg);
int receiveMsgFromQueueTriggerUS(msgTriggerUS* inMsg);

#endif
