#ifndef STATTIMER_H_
#define STATTIMER_H_

#include <stddef.h>

#include <ti/drivers/Timer.h>

#include "ti_drivers_config.h"

//#include "debug.h"
#include "queues.h"
#include "mqtt_client_app.h"


void init_statsTimer();
void init_pubTimer();

/*
 * Send a message to the stats queue which will trigger it to push the current data to the broker
 */
void statsTimerCallback(Timer_Handle myHandle, int_fast16_t status);

/*
 * Send a message to the generic task which will trigger it to publish a new message
 */
void pubTimerCallback(Timer_Handle myHandle, int_fast16_t status);

#endif /* STATTIMER_H_ */
