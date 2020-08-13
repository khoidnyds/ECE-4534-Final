#include "queues.h"



int init_queue(){
    xQueue_mqtt  = NULL;
    xQueue_gen   = NULL;
    xQueueTriggerRGBSwitch = NULL;

    xQueue_mqtt = xQueueCreate(QUEUESIZE, sizeof(mqttMsg));
    xQueue_gen = xQueueCreate(QUEUESIZE, sizeof(unpackedMsg));
    xQueueTriggerRGBSwitch = xQueueCreate(QUEUESIZE, sizeof(msgTriggerRGBSwitch));

    if (xQueue_mqtt==NULL || xQueue_gen==NULL || xQueueTriggerRGBSwitch == NULL)
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

int sendMsgToQueueTriggerRGBSwitch(msgTriggerRGBSwitch* outMsg){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t result = xQueueSendToBackFromISR(xQueueTriggerRGBSwitch, outMsg, &xHigherPriorityTaskWoken);
    if (result != pdTRUE){
        //dbgOutputLoc(DLOC_MESSAGE_SWITCH_SEND_FAIL);
        return -1;
    }
    //dbgOutputLoc(DLOC_MESSAGE_SWITCH_SEND_SUCCESS);
return 0;
}

int receiveMsgFromQueueTriggerRGBSwitch(msgTriggerRGBSwitch* inMsg){
    BaseType_t result = xQueueReceive(xQueueTriggerRGBSwitch, inMsg, portMAX_DELAY);
    if (result != pdPASS){
        //dbgOutputLoc(DLOC_MESSAGE_SWITCH_RECEIVE_FAIL);
        return -1;
    }
    //dbgOutputLoc(DLOC_MESSAGE_SWITCH_RECEIVE_SUCCESS);
    return 0;
}
