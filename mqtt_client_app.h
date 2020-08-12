/*
 * mqtt_client_app.h
 *
 *  Created on: Jul 16, 2020
 *      Author: Matthew
 */

#ifndef MQTT_CLIENT_APP_H_
#define MQTT_CLIENT_APP_H_


/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <mqtt_if.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>

#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/slnetifwifi.h>

#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

#include <ti/net/mqtt/mqttclient.h>

#include "ti_drivers_config.h"

#include "network_if.h"
#include "uart_term.h"
#include "mqtt_if.h"
#include "debug_if.h"
#include "setup_util.h"
#include "queues.h"
#include "pubTimer.h"
#include "generic_task.h"
#include "timerRGB.h"

extern int32_t ti_net_SlNet_initConfig();

#define APPLICATION_NAME         "MQTT client"
#define APPLICATION_VERSION      "2.0.0"

#define SL_TASKSTACKSIZE            2048
#define SPAWN_TASK_PRIORITY         9

#define MQTT_MODULE_TASK_PRIORITY   2
#define MQTT_MODULE_TASK_STACK_SIZE 2048

#define MQTT_WILL_TOPIC             "/team3/cc32xx_will_topic"
#define MQTT_WILL_MSG               "will_msg_works"
#define MQTT_WILL_QOS               MQTT_QOS_2
#define MQTT_WILL_RETAIN            false

#define MQTT_CLIENT_PASSWORD        "cc3220sfSummer"
#define MQTT_CLIENT_USERNAME        "team3"
#define MQTT_CLIENT_KEEPALIVE       0
#define MQTT_CLIENT_CLEAN_CONNECT   true
#define MQTT_CLIENT_MQTT_V3_1       true
#define MQTT_CLIENT_BLOCKING_SEND   true

#define MQTT_CONNECTION_FLAGS           MQTTCLIENT_NETCONN_URL
#define MQTT_CONNECTION_ADDRESS         "ip.mjforan.com"
#define MQTT_CONNECTION_PORT_NUMBER     2883

#define QOS MQTT_QOS_0

//////////// Change this to select which board you are using
#define BOARD_1

#ifdef BOARD_0
#define THIS_BOARD_ID           "b0"
#define OTHER_BOARD_ID_0        "b1"
#define OTHER_BOARD_ID_1        "b2"
#define OTHER_BOARD_ID_2        "b3"
#endif
#ifdef BOARD_1
#define THIS_BOARD_ID           "b1"
#define OTHER_BOARD_ID_0        "b0"
#define OTHER_BOARD_ID_1        "b2"
#define OTHER_BOARD_ID_2        "b3"
#endif
#ifdef BOARD_2
#define THIS_BOARD_ID           "b2"
#define OTHER_BOARD_ID_0        "b1"
#define OTHER_BOARD_ID_1        "b0"
#define OTHER_BOARD_ID_2        "b3"
#endif
#ifdef BOARD_3
#define THIS_BOARD_ID           "b3"
#define OTHER_BOARD_ID_0        "b1"
#define OTHER_BOARD_ID_1        "b2"
#define OTHER_BOARD_ID_2        "b0"
#endif

#define NUM_SUB_TOPICS           4
#define SUB_TOPIC_0             "/team3/"OTHER_BOARD_ID_0"/out"
#define SUB_TOPIC_1             "/team3/"OTHER_BOARD_ID_1"/out"
#define SUB_TOPIC_2             "/team3/"OTHER_BOARD_ID_2"/out"
#define SUB_TOPIC_3             "/team3/"THIS_BOARD_ID"/in"

#define NUM_PUB_TOPICS           7
#define PUB_TOPIC_US1            "/team3/sensorsK/us1"
#define PUB_TOPIC_US2            "/team3/sensorsK/us2"
#define PUB_TOPIC_US3            "/team3/sensorsK/us3"
#define PUB_TOPIC_US4            "/team3/sensorsK/us4"
#define PUB_TOPIC_RGB            "/team3/sensorsK/rgb"
#define PUB_TOPIC_SWITCH         "/team3/sensorsK/switch"

#define ALL_TOPIC_COUNT          (NUM_SUB_TOPICS + NUM_PUB_TOPICS)

#define STATS_PUB_TOPIC          "/team3/sensors/stats"

extern char* allTopics[ALL_TOPIC_COUNT];

#endif /* MQTT_CLIENT_APP_H_ */
