#include "timerRGB.h"

void initTimerRGB()
{
    Timer_Handle timerRGB;
    Timer_Params params;

    Timer_init();
    Timer_Params_init(&params);
    params.period = Timer_RGB_PERIOD_IN_US;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerRGBCallback;
    timerRGB = Timer_open(CONFIG_TIMER_2, &params);

    if (timerRGB == NULL) {
        errorHalt("Timer RGB open failed");
    }

    if (Timer_start(timerRGB) == Timer_STATUS_ERROR) {
        errorHalt("Timer RGB start failed");
    }
    seqNum_switch=0;
    return;
}

void timerRGBCallback(Timer_Handle myHandle, int_fast16_t status)
{
    msgTriggerRGBSwitch newTriggerRGBSwitch;
    sendMsgToQueueTriggerRGBSwitch(&newTriggerRGBSwitch);

    unpackedMsg outMsg;

    if(GPIO_read(LIMIT_SWITCH)==LOW){
        strcpy(outMsg.payload, "yes");
    }
    else{
        strcpy(outMsg.payload, "no");
    }

    strcpy(outMsg.topic, PUB_TOPIC_SWITCH);

//    int success = sendToGenQueueIsr(&outMsg);
//    if(success){
//        Message("\r\nSend Switch message failed");
//        while(1);
//    }

    mqttMsg sendMsg;
    sendMsg.event = APP_MQTT_PUBLISH;
    strcpy(sendMsg.topic, outMsg.topic);
    outMsg.timestamp=(portTICK_PERIOD_MS*xTaskGetTickCount())/1000.0;
    outMsg.sequenceNum=seqNum_switch;
    strcpy(sendMsg.payload, "[");
    json_pack(&outMsg, sendMsg.payload);
    strcat(sendMsg.payload, "]");
    seqNum_switch++;

    sendToMqttQueueIsr(&sendMsg);
}



