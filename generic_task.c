#include <generic_task.h>

void* genTask(void* args){

    dbgOutputLoc(DLOC_GT_START);

    while(1){
        unpackedMsg outMsg;
        mqttMsg sendMsg;
        sendMsg.event = APP_MQTT_PUBLISH;

        dbgOutputLoc(DLOC_GT_WHILE_START);

        receiveFromGenQueue(&outMsg);

        dbgOutputLoc(DLOC_GT_MAKEMSG_START);
        outMsg.timestamp=(portTICK_PERIOD_MS*xTaskGetTickCount())/1000.0;
        dbgOutputLoc(DLOC_GT_MAKEMSG_TS);

        if(!strcmp(outMsg.topic, PUB_TOPIC_US_FRONT)){
            outMsg.sequenceNum=seqNum_us1;
            seqNum_us1++;
        }
        else if(!strcmp(outMsg.topic, PUB_TOPIC_US_LEFT)){
            outMsg.sequenceNum=seqNum_us2;
            seqNum_us2++;
        }
        else if(!strcmp(outMsg.topic, PUB_TOPIC_US_RIGHT)){
            outMsg.sequenceNum=seqNum_us3;
            seqNum_us3++;
        }
        else if(!strcmp(outMsg.topic, PUB_TOPIC_SWITCH)){
            outMsg.sequenceNum=seqNum_switch;
            seqNum_switch++;
        }
        else if(!strcmp(outMsg.topic, PUB_TOPIC_RGB)){
            outMsg.sequenceNum=seqNum_rgb;
            seqNum_rgb++;
        }

        dbgOutputLoc(DLOC_GT_MAKEMSG_SN);

        strcpy(sendMsg.payload, "[");
        json_pack(&outMsg, sendMsg.payload);
        dbgOutputLoc(DLOC_GT_MSG_PACKED);
        strcat(sendMsg.payload, "]");
        dbgOutputLoc(DLOC_GT_MAKEMSG_FINISH);

        //strcpy(sendMsg.payload, outMsg.payload);
        strcpy(sendMsg.topic, outMsg.topic);

        sendToMqttQueue(&sendMsg);

        dbgOutputLoc(DLOC_GT_WHILE_EXIT);
        }
}
