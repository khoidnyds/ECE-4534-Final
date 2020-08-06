#include <us_task.h>

void* usTask(void* args){
    currentState = INIT;
    currentInt = START;
    start=0;
    end=0;
    while(1){
        msgTriggerUS newMsgTriggerUS;
        receiveMsgFromQueueTriggerUS(&newMsgTriggerUS);
        if(newMsgTriggerUS.triggerUS){
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
    }
}
void fsm(uint_least8_t pinin, uint_least8_t pinout){
    GPIO_setConfig(pinin, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_write(pinin, HIGH);
    int i;
    for(i=0;i<30;i++);
    GPIO_write(pinin, LOW);

    GPIO_setCallback(pinout, getTime);
    GPIO_enableInt(pinout);
}

void getTime(uint_least8_t index){
    switch(currentInt){
    case START:{
        currentInt = END;
        start = Timer_getCount(timerUS);
        break;
    }
    case END:{
        currentInt = START;
        end = Timer_getCount(timerUS);

        double speed = (331 + 0.6 * AMBIENT_TEMP) / 1000;
        int distance = speed * (end-start) / 16; //80MHz and 2*distance
        msgUS newMsgUS;
        switch(index){
            case US1_ECHO:
                newMsgUS.position = left;
                break;
            case US2_ECHO:
                newMsgUS.position = front;
                break;
            case US3_ECHO:
                newMsgUS.position = right;
                break;
            case US4_ECHO:
                newMsgUS.position = back;
                break;
            default:
                break;
        }
        newMsgUS.distance = distance;
        newMsgUS.type = ultrasonic;
        int success = sendMsgToQueueUS(&newMsgUS);
        if(success==FAIL){
            Message("\r\nSend US message failed");
            while(1);
        }

        start=0;
        end=0;
        break;
    }
    default:
        break;
    }
}
void echoCallback(Timer_Handle handle, int_fast16_t status){
    // do nothing
}
