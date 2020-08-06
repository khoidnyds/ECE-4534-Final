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



            msgUS newMsg;
            receiveMsgFromQueueUS(&newMsg);
            char buffer[sizeof(unsigned int)];
            sprintf(buffer, "%d", newMsg.distance);
            strcpy(outMsg.payload, buffer);

            switch(newMsg.position){
            case left:
                outMsg.sequenceNum=seqNum_us1;
                dbgOutputLoc(DLOC_GT_MAKEMSG_SN);
                seqNum_us1++;
                strcpy(outMsg.topic, PUB_TOPIC_0);
                dbgOutputLoc(DLOC_GT_MAKEMSG_TOPIC);
                break;
            case front:
                outMsg.sequenceNum=seqNum_us2;
                dbgOutputLoc(DLOC_GT_MAKEMSG_SN);
                seqNum_us2++;
                strcpy(outMsg.topic, PUB_TOPIC_1);
                dbgOutputLoc(DLOC_GT_MAKEMSG_TOPIC);
                break;
            case right:
                outMsg.sequenceNum=seqNum_us3;
                dbgOutputLoc(DLOC_GT_MAKEMSG_SN);
                seqNum_us3++;
                strcpy(outMsg.topic, PUB_TOPIC_2);
                dbgOutputLoc(DLOC_GT_MAKEMSG_TOPIC);
                break;
            case back:
                outMsg.sequenceNum=seqNum_us4;
                dbgOutputLoc(DLOC_GT_MAKEMSG_SN);
                seqNum_us4++;
                strcpy(outMsg.topic, PUB_TOPIC_3);
                dbgOutputLoc(DLOC_GT_MAKEMSG_TOPIC);
                break;
            default:
                break;
            }


            dbgOutputLoc(DLOC_GT_MAKEMSG_PAYLOAD);
            outMsg.statsCmd = PUBLISHED;

            outMsg.msgType = GENERAL;
            strcpy(sendMsg.payload, "[");
            json_pack(&outMsg, sendMsg.payload);
            dbgOutputLoc(DLOC_GT_MSG_PACKED);
            strcat(sendMsg.payload, "]");
            dbgOutputLoc(DLOC_GT_MAKEMSG_FINISH);
            strcpy(sendMsg.topic, outMsg.topic);
            sendToMqttQueue(&sendMsg);

            dbgOutputLoc(DLOC_GT_STAT_SEND);
            if(sendToStatsQueue(&outMsg)){
                errorHalt("error adding to queue");
                dbgOutputLoc(DLOC_GT_ERROR);
            }
        }
    }
    //dbgOutputLoc(DLOC_GT_WHILE_EXIT);
}
