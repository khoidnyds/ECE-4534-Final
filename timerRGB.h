#ifndef TIMERRGB_H_
#define TIMERRGB_H_

#include <queues.h>
#include <ti/drivers/Timer.h>
#include "mqtt_client_app.h"

#define Timer_RGB_PERIOD_IN_US  1000000
#define LOW      0


int seqNum_switch;

void initTimerRGB();
void timerRGBCallback(Timer_Handle myHandle, int_fast16_t status);
#endif
