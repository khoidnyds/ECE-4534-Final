#ifndef STATTIMER_H_
#define STATTIMER_H_

#include <stddef.h>
#include <ti/drivers/Timer.h>
#include "ti_drivers_config.h"
#include <ti/drivers/GPIO.h>

#define Timer_US_PERIOD_IN_US   100000
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
typedef enum {INIT, STATE_US_FRONT, STATE_US_LEFT, STATE_US_RIGHT} state;
state currentState;
typedef enum {START, END} GPIO_int;
GPIO_int currentInt;


void initTimerUS();
void usTrigger(Timer_Handle handle, int_fast16_t status);
/*
 * Send a message to the stats queue which will trigger it to push the current data to the broker
 */
void getTime(uint_least8_t index);
void fsm(uint_least8_t pinin, uint_least8_t pinout);

#endif /* STATTIMER_H_ */
