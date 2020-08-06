#include "stats.h"



void* statsTask(void *arg0){
    //track separate statistics for each topic
    int numPublished[ALL_TOPIC_COUNT];
    int numReceived[ALL_TOPIC_COUNT];
    int numDropped[ALL_TOPIC_COUNT];
    int lastSeqNum[ALL_TOPIC_COUNT];

    //initialize all statistics to zero
    int i;
    for (i = 0; i < ALL_TOPIC_COUNT; i++) {
        numPublished[i]= 0;
        numReceived[i] = 0;
        numDropped[i]  = 0;
        lastSeqNum[i]  = 0;
    }
    mqttMsg sendMsg;
    unpackedMsg outMsg;
    strcpy(sendMsg.topic, STATS_PUB_TOPIC);
    sendMsg.event=APP_MQTT_PUBLISH;
    unpackedMsg newVal;

    while (1) {
        if(receiveFromStatsQueue(&newVal)<0){
            errorHalt("Error receiving from stats queue!\n\r");
        }

        //Associate the topic of the received message with an array position for a known topic
        int topicIndex = -1;
        if(newVal.statsCmd!=PUSH){
            for(i=0; i<ALL_TOPIC_COUNT; i++){
                if(strcmp(newVal.topic, allTopics[i])==0){
                    topicIndex=i;
                    break;
                }
            }
            if(topicIndex<0){
                LOG_ERROR("Unknown topic: %s", newVal.topic);
                continue;
                //errorHalt("Unknown Topic");
            }
        }

        switch(newVal.statsCmd){
        case PUBLISHED:
            numPublished[topicIndex]++;
            break;

        case RECEIVED:
            numReceived[topicIndex]++;
            //if the sequence number decreased or stayed the same, assume the board reset and restart the statistics
            if(newVal.sequenceNum<=lastSeqNum[topicIndex]){
                numPublished[topicIndex]= 0;
                numReceived[topicIndex] = 1;
                numDropped[topicIndex]  = 0;
            }
            else{
                //increment the number of messages missed based on the current and previous sequence numbers
                numDropped[topicIndex]+=newVal.sequenceNum-(lastSeqNum[topicIndex]+1);
            }
            //update the last sequence number seen
            lastSeqNum[topicIndex]=newVal.sequenceNum;
            break;

        case PUSH:
            strcpy(sendMsg.payload, "[");            //start of JSON array
            for (i=0; i<ALL_TOPIC_COUNT; i++){      //pack the stats for each topic in their own objects
                outMsg.msgType=STATS;
                strcpy(outMsg.topic, allTopics[i]);
                outMsg.numPub=numPublished[i];
                outMsg.numRec=numReceived[i];
                outMsg.numDrop=numDropped[i];
                json_pack(&outMsg, sendMsg.payload); //after copying all statistics into a struct, pack it up into a JSON object
                if(i<ALL_TOPIC_COUNT-1)
                    strcat(sendMsg.payload, ", ");   //if this is not the last topic, add a comma separator
            }

            strcat(sendMsg.payload, "]");            //close the JSON list
            sendToMqttQueue(&sendMsg);               //send it off to be published
            break;

        default:
            errorHalt("Unknown stats value");
            break;
        }

    }
}


