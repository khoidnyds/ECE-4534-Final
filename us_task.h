#ifndef US_TASK_H_
#define US_TASK_H_

#include <pthread.h>

#include "mqtt_client_app.h"
#include "pubTimer.h"
#include "queues.h"
#include "json_parser.h"
#include "debug_if.h"

void* usTask(void* args);


#endif /* GENERIC_TASK_H_ */
