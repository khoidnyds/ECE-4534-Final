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

void init_pubTimer(){
    Timer_Handle timer1;
    Timer_Params params;
    Timer_init();
    Timer_Params_init(&params);
    params.period = 5;
    params.periodUnits = Timer_PERIOD_HZ;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = pubTimerCallback;
    timer1 = Timer_open(CONFIG_TIMER_1, &params);
    if (timer1 == NULL) {
        dbgOutputLoc(DLOC_P_PUBTIMER_NULL);
        errorHalt("Timer1 init failed");
    }
    if (Timer_start(timer1) == Timer_STATUS_ERROR) {
        dbgOutputLoc(DLOC_P_PUBTIMER_STATERR);
        errorHalt("Timer1 init failed");
    }
    dbgOutputLoc(DLOC_P_PUB_TIMER_INITD);
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

void pubTimerCallback(Timer_Handle myHandle, int_fast16_t status){
    dbgOutputLoc(DLOC_P_PUB_TIMER_CALL_START);
    unpackedMsg outMsg;
    outMsg.statsCmd = PUBLISHED;
    int result = sendToGenQueueIsr(&outMsg);
    if(result<0){
        dbgOutputLoc(DLOC_P_STAT_TIMER_CALL_FAIL_PUBQ);
        errorHalt("Unable to add to pub queue");
    }
    dbgOutputLoc(DLOC_P_PUB_TIMER_CALL_SUCC);
}

