#include <generic_task.h>

void* genTask(void* args){

    dbgOutputLoc(DLOC_GT_START);
    int seqNum_us1 = 0;
    int seqNum_us2 = 0;
    int seqNum_us3 = 0;
    int seqNum_us4 = 0;
    unpackedMsg outMsg;
    mqttMsg sendMsg;
    sendMsg.event = APP_MQTT_PUBLISH;

    while(1){
        dbgOutputLoc(DLOC_GT_WHILE_START);

        receiveFromGenQueue(&outMsg);

        if(outMsg.statsCmd==RECEIVED){
            dbgOutputLoc(DLOC_GT_RECEIVED);
            LOG_INFO("TOPIC: %s \tPAYLOAD: %s\r\n", outMsg.topic, outMsg.payload);
        }
        else{
            dbgOutputLoc(DLOC_GT_MAKEMSG_START);
            outMsg.timestamp=(portTICK_PERIOD_MS*xTaskGetTickCount())/1000.0;
            dbgOutputLoc(DLOC_GT_MAKEMSG_TS);

            if(strcmp(outMsg.topic, PUB_TOPIC_US1)){
                outMsg.sequenceNum=seqNum_us1;
                seqNum_us1++;
            }
            else if(strcmp(outMsg.topic, PUB_TOPIC_US2)){
                outMsg.sequenceNum=seqNum_us2;
                seqNum_us2++;
            }
            else if(strcmp(outMsg.topic, PUB_TOPIC_US3)){
                outMsg.sequenceNum=seqNum_us3;
                seqNum_us3++;
            }
            else if(strcmp(outMsg.topic, PUB_TOPIC_US4)){
                outMsg.sequenceNum=seqNum_us4;
                seqNum_us4++;
            }
            dbgOutputLoc(DLOC_GT_MAKEMSG_SN);

            strcpy(sendMsg.payload, "[");
            json_pack(&outMsg, sendMsg.payload);
            dbgOutputLoc(DLOC_GT_MSG_PACKED);
            strcat(sendMsg.payload, "]");
            dbgOutputLoc(DLOC_GT_MAKEMSG_FINISH);
            strcpy(sendMsg.topic, outMsg.topic);


            sendToMqttQueue(&sendMsg);


            dbgOutputLoc(DLOC_GT_STAT_SEND);
//            if(sendToStatsQueue(&outMsg)){
//                errorHalt("error adding to queue");
//                dbgOutputLoc(DLOC_GT_ERROR);
//            }
        }
    }
    //dbgOutputLoc(DLOC_GT_WHILE_EXIT);
}
