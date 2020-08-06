#include "json_parser.h"

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  return !(tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start && strncmp(json + tok->start, s, tok->end - tok->start) == 0);
}

int json_unpack(unpackedMsg* unpacked, char* inStr){
    static int init = 0;
    static jsmn_parser parser;
    if(!init)
        jsmn_init(&parser);


    dbgOutputLoc(DLOC_J_UNPACK_START);
    jsmntok_t tokens[MQTT_PAYLOAD_SIZE];
    int r = jsmn_parse(&parser, inStr, strlen(inStr), tokens, MQTT_PAYLOAD_SIZE);
    if (r < 0){
       UART_PRINT("\n\rFailed to parse JSON: %d", r);
       return -1;
    }
    dbgOutputLoc(DLOC_J_UNPACK_PARSE_SUCC);
    int i; //ignore the first two elements: list, object   [{
    for(i = 2; i < (sizeof(tokens) / sizeof(tokens[1])); i+=2)
    {
        dbgOutputLoc(DLOC_J_UNPACK_FOR_ENTER);
        if(tokens[i].type==JSMN_UNDEFINED){
            break;
        }
        if (jsoneq(inStr, &tokens[i], JSON_TIMESTAMP) == 0)         //extract the timestamp element
            unpacked->timestamp = strToDouble(inStr, tokens[i+1].start, tokens[i+1].end - tokens[i+1].start);
        else if (jsoneq(inStr, &tokens[i], JSON_SEQUENCE_NUM) == 0) //extract the sequence number
            unpacked->sequenceNum = (int)strToDouble(inStr, tokens[i+1].start, tokens[i+1].end - tokens[i+1].start);
        else if (jsoneq(inStr, &tokens[i], JSON_PAYLOAD) == 0)      //extract the payload string
            strncpy(unpacked->payload, inStr+tokens[i+1].start, tokens[i+1].end - tokens[i+1].start);
        else
            return -1;
        dbgOutputLoc(DLOC_J_UNPACK_FOR_FINISH);
    }
    return 0;
}


int json_pack(unpackedMsg* unpacked, char* outStr){
    dbgOutputLoc(DLOC_J_PACK_START);

    // {"ts":4530.40, "sn":22639, "pload":"payload string"}
    if(unpacked->msgType==GENERAL){
        strcat(outStr, "{\"ts\":");
        doubleToStr(unpacked->timestamp, outStr+strlen(outStr), 2);
        strcat(outStr, ", \"sn\":");
        intToString(unpacked->sequenceNum, outStr+strlen(outStr));
        strcat(outStr, ", \"pload\":\"");
        strcat(outStr, unpacked->payload);
        strcat(outStr, "\"}");
    }
    //{"topic":"/team3/b2/out", "#pub":0, "#rec":120, "#drop":1}
    else if(unpacked->msgType==STATS){
        strcat(outStr, "{\"topic\":\"");
        strcat(outStr, unpacked->topic);
        strcat(outStr, "\", \"#pub\":");
        intToString(unpacked->numPub, outStr+strlen(outStr));
        strcat(outStr, ", \"#rec\":");
        intToString(unpacked->numRec, outStr+strlen(outStr));
        strcat(outStr, ", \"#drop\":");
        intToString(unpacked->numDrop, outStr+strlen(outStr));
        strcat(outStr, "}");
    }
    else{
        return -1;
    }
    dbgOutputLoc(DLOC_J_PACK_FINISH);
    return 0;
}

