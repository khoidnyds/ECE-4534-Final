#ifndef US_TASK_H_
#define US_TASK_H_

#include <pthread.h>
#include "FreeRTOS.h"
#include "mqtt_client_app.h"
#include "pubTimer.h"
#include "queues.h"
#include "json_parser.h"
#include "debug_if.h"

#define AMBIENT_TEMP            30 // in Celcius
#define HIGH                    1
#define LOW                     0
#define SUCCESS                 0
#define FAIL                    -1
#define MAX_COUNT               3 //sensor update every 0.84s
#define Timer_ECHO_PERIOD_IN_US 60000 //big enough to NOT be roll-over

double start, end;
double us1_total, us2_total, us3_total, us4_total;
int us1_count, us2_count, us3_count, us4_count;
typedef enum {INIT, STATE_US1, STATE_US2, STATE_US3, STATE_US4} state;
state currentState;
typedef enum {START, END} GPIO_int;
GPIO_int currentInt;

void* usTask(void *arg0);
void getTime(uint_least8_t index);
void fsm(uint_least8_t pinin, uint_least8_t pinout);
void echoCallback(Timer_Handle handle, int_fast16_t status);



#endif /* GENERIC_TASK_H_ */
