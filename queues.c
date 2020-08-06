#include "queues.h"

QueueHandle_t xQueue_mqtt  = NULL;
QueueHandle_t xQueue_stats = NULL;
QueueHandle_t xQueue_gen   = NULL;

int init_queue(){
    xQueue_mqtt = xQueueCreate(QUEUESIZE, sizeof(mqttMsg));
    xQueue_stats = xQueueCreate(QUEUESIZE, sizeof(unpackedMsg));
    xQueue_gen = xQueueCreate(QUEUESIZE, sizeof(unpackedMsg));

    if (xQueue_mqtt==NULL || xQueue_stats==NULL || xQueue_gen==NULL)
        return -1;
    dbgOutputLoc(DLOC_Q_INIT_SUCC);
    return 0;
}
int sendToMqttQueueIsr(mqttMsg* outMsg){
    dbgOutputLoc(DLOC_Q_SEND_MQ_START);
    BaseType_t result = xQueueSendToBackFromISR(xQueue_mqtt, outMsg, NULL);
    if (result != pdTRUE)
        return -1;
    dbgOutputLoc(DLOC_Q_SEND_MQ_SUCC);
    return 0;
}
int sendToMqttQueue(mqttMsg* outMsg){
    dbgOutputLoc(DLOC_Q_SEND_MQ_START);
    BaseType_t result = xQueueSendToBack(xQueue_mqtt, outMsg, portMAX_DELAY);
    if (result != pdTRUE)
        return -1;
    dbgOutputLoc(DLOC_Q_SEND_MQ_SUCC);
    return 0;
}

int receiveFromMqttQueue(mqttMsg* inMsg){
    dbgOutputLoc(DLOC_Q_REC_MQ_START);
    BaseType_t result = xQueueReceive(xQueue_mqtt, inMsg, portMAX_DELAY);
    if (result != pdPASS)
        return -1;
    dbgOutputLoc(DLOC_Q_REC_MQ_SUCC);
    return 0;
}

int sendToStatsQueueIsr(unpackedMsg* outMsg){
    dbgOutputLoc(DLOC_Q_SEND_STAT_START);
    BaseType_t result = xQueueSendToBackFromISR(xQueue_stats, outMsg, NULL);
    if (result != pdTRUE)
        return -1;
    dbgOutputLoc(DLOC_Q_SEND_STAT_SUCC);
    return 0;
}
int sendToStatsQueue(unpackedMsg* outMsg){
    dbgOutputLoc(DLOC_Q_SEND_STAT_START);
    BaseType_t result = xQueueSendToBack(xQueue_stats, outMsg, portMAX_DELAY);
    if (result != pdTRUE)
        return -1;
    dbgOutputLoc(DLOC_Q_SEND_STAT_SUCC);
    return 0;
}


int receiveFromStatsQueue(unpackedMsg* inMsg){
    dbgOutputLoc(DLOC_Q_REC_STAT_START);
    BaseType_t result = xQueueReceive(xQueue_stats, inMsg, portMAX_DELAY);
    if (result != pdPASS)
        return -1;
    dbgOutputLoc(DLOC_Q_REC_STAT_SUCC);
    return 0;
}

int sendToGenQueueIsr(unpackedMsg* outMsg){
    dbgOutputLoc(DLOC_Q_SEND_GEN_START);
    BaseType_t result = xQueueSendToBackFromISR(xQueue_gen, outMsg, NULL);
    if (result != pdTRUE)
        return -1;
    dbgOutputLoc(DLOC_Q_SEND_GEN_SUCC);
    return 0;
}


int sendToGenQueue(unpackedMsg* outMsg){
    dbgOutputLoc(DLOC_Q_SEND_GEN_START);
    BaseType_t result = xQueueSendToBack(xQueue_gen, outMsg, portMAX_DELAY);
    if (result != pdTRUE)
        return -1;
    dbgOutputLoc(DLOC_Q_SEND_GEN_SUCC);
    return 0;
}

int receiveFromGenQueue(unpackedMsg* inMsg){
    dbgOutputLoc(DLOC_Q_REC_GEN_START);
    BaseType_t result = xQueueReceive(xQueue_gen, inMsg, portMAX_DELAY);
    if (result != pdPASS)
        return -1;
    dbgOutputLoc(DLOC_Q_REC_GEN_SUCC);
    return 0;

}
