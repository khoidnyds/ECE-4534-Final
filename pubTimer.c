#include "pubTimer.h"


void init_statsTimer(){
    Timer_Handle timer0;
    Timer_Params params;
    Timer_init();
    Timer_Params_init(&params);
    params.period = 1;
    params.periodUnits = Timer_PERIOD_HZ;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = statsTimerCallback;
    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL){
        errorHalt("Timer0 init failed");
    }
    if (Timer_start(timer0) == Timer_STATUS_ERROR){
        errorHalt("Timer0 init failed");
    }
    dbgOutputLoc(DLOC_P_STATS_TIMER_INITD);
}

void statsTimerCallback(Timer_Handle myHandle, int_fast16_t status){
    dbgOutputLoc(DLOC_P_STAT_TIMER_CALL_START);
    unpackedMsg msg;
    msg.statsCmd = PUSH;
    int result = sendToStatsQueueIsr(&msg);
    if(result<0){
        dbgOutputLoc(DLOC_P_STAT_TIMER_CALL_FAIL_STATQ);
        errorHalt("Unable to add to stats queue");
    }
    dbgOutputLoc(DLOC_P_STAT_TIMER_CALL_SUCC);
}

void initTimerUS()
{
    start=0;
    end=0;
    currentInt = START;
    currentState = INIT;

    Timer_Params params;

    Timer_init();
    Timer_Params_init(&params);
    params.period = Timer_US_PERIOD_IN_US;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = usTrigger;

    Message("\r\nUSTO");
    timerUS = Timer_open(CONFIG_TIMER_2, &params);

    if (timerUS == NULL) {
        while(1);
    }

    Message("\r\nUSTS");
    if (Timer_start(timerUS) == Timer_STATUS_ERROR) {
        while(1);
    }
}

void usTrigger(Timer_Handle handle, int_fast16_t status){
    switch(currentState){
        case INIT:
            fsm(US1_TRIGGER, US1_ECHO);
            currentState = STATE_US1;
            break;
        case STATE_US1:
            fsm(US2_TRIGGER, US2_ECHO);
            currentState = STATE_US2;
            break;
        case STATE_US2:
            fsm(US3_TRIGGER, US3_ECHO);
            currentState = STATE_US3;
            break;
        case STATE_US3:
            fsm(US4_TRIGGER, US4_ECHO);
            currentState = STATE_US4;
            break;
        case STATE_US4:
            fsm(US1_TRIGGER, US1_ECHO);
            currentState = STATE_US1;
            break;
        default:
            break;
    }
}

void fsm(uint_least8_t pinin, uint_least8_t pinout){
    GPIO_enableInt(pinout);

    GPIO_write(pinin, HIGH);
    int i;
    for(i=0;i<30;i++);
    GPIO_write(pinin, LOW);

}

void getTime(uint_least8_t index){
    if(currentInt== START){
        currentInt = END;
        start = Timer_getCount(timerUS);
    }
    else {
        currentInt = START;
        end = Timer_getCount(timerUS);

        double speed = (331 + 0.6 * AMBIENT_TEMP) / 1000;
        unsigned int distance = speed * (end-start) / 16 * 10; //80MHz and 2*distance

        unpackedMsg outMsg;
        switch(index){
            case US1_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US1);
                break;
            case US2_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US2);
                break;
            case US3_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US3);
                break;
            case US4_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US4);
                break;
            default:
                break;
        }
        dbgOutputLoc(DLOC_GT_MAKEMSG_TOPIC);

        char buffer[sizeof(unsigned int)];
        sprintf(buffer, "%d", distance);
        strcpy(outMsg.payload, buffer);
        dbgOutputLoc(DLOC_GT_MAKEMSG_PAYLOAD);

        outMsg.statsCmd = PUBLISHED;
        outMsg.msgType = GENERAL;

        int success = sendToGenQueueIsr(&outMsg);
        if(success==FAIL){
            Message("\r\nSend US message failed");
            while(1);
        }

        start=0;
        end=0;
    }
}

