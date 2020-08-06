#ifndef GENERIC_TASK_H_
#define GENERIC_TASK_H_

#include <pthread.h>

#include "mqtt_client_app.h"
#include "pubTimer.h"
#include "queues.h"
#include "json_parser.h"
#include "debug_if.h"

void* genTask(void* args);


#endif /* GENERIC_TASK_H_ */
