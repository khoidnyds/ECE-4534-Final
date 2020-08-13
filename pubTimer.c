#include "pubTimer.h"


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

    timerUS = Timer_open(CONFIG_TIMER_1, &params);

    if (timerUS == NULL) {
        while(1);
    }

    if (Timer_start(timerUS) == Timer_STATUS_ERROR) {
        while(1);
    }
}

void usTrigger(Timer_Handle handle, int_fast16_t status){
    switch(currentState){
        case INIT:
            fsm(US_FRONT_TRIGGER, US_FRONT_ECHO);
            currentState = STATE_US_FRONT;
            break;
        case STATE_US_FRONT:
            fsm(US_LEFT_TRIGGER, US_LEFT_ECHO);
            currentState = STATE_US_LEFT;
            break;
        case STATE_US_LEFT:
            fsm(US_RIGHT_TRIGGER, US_RIGHT_ECHO);
            currentState = STATE_US_RIGHT;
            break;
        case STATE_US_RIGHT:
            fsm(US_FRONT_TRIGGER, US_FRONT_ECHO);
            currentState = STATE_US_FRONT;
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
        unsigned int distance;
        if(end>start)
            distance = speed * (end-start) / 16 * 10; //80MHz and 2*distance
        else
            distance = speed * (start-end) / 16 * 10;
        unpackedMsg outMsg;
        switch(index){
            case US_FRONT_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US_FRONT);
                break;
            case US_LEFT_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US_LEFT);
                break;
            case US_RIGHT_ECHO:
                strcpy(outMsg.topic, PUB_TOPIC_US_RIGHT);
                break;
            default:
                break;
        }
        dbgOutputLoc(DLOC_GT_MAKEMSG_TOPIC);

        char buffer[sizeof(unsigned int)];
        sprintf(buffer, "%d", distance);
        strcpy(outMsg.payload, buffer);
        dbgOutputLoc(DLOC_GT_MAKEMSG_PAYLOAD);


        int success = sendToGenQueueIsr(&outMsg);
        if(success==FAIL){
            Message("\r\nSend US message failed");
            while(1);
        }

        start=0;
        end=0;
    }
}

