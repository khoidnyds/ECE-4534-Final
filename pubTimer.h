#ifndef STATTIMER_H_
#define STATTIMER_H_

#include <stddef.h>
#include <ti/drivers/Timer.h>
#include "ti_drivers_config.h"
#include <ti/drivers/GPIO.h>

#define Timer_US_PERIOD_IN_US   70000 //70ms
#define AMBIENT_TEMP            30 // in Celcius
#define HIGH                    1
#define LOW                     0
#define SUCCESS                 0
#define FAIL                    -1
//#include "debug.h"
#include "queues.h"
#include "mqtt_client_app.h"

double start, end;
Timer_Handle timerUS;
typedef enum {INIT, STATE_US1, STATE_US2, STATE_US3, STATE_US4} state;
state currentState;
typedef enum {START, END} GPIO_int;
GPIO_int currentInt;

void init_statsTimer();

void initTimerUS();
void usTrigger(Timer_Handle handle, int_fast16_t status);
/*
 * Send a message to the stats queue which will trigger it to push the current data to the broker
 */
void statsTimerCallback(Timer_Handle myHandle, int_fast16_t status);
void getTime(uint_least8_t index);
void fsm(uint_least8_t pinin, uint_least8_t pinout);

#endif /* STATTIMER_H_ */
