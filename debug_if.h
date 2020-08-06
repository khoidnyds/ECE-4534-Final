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

//*****************************************************************************
// Includes
//*****************************************************************************
// Standard includes

#ifndef DEBUG_IF_H
#define DEBUG_IF_H






// TI-Driver includes
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h> //need to include GPIO
#include <FreeRTOS.h>//possible include RTOS header to disable interrupts
#include <task.h>
#include <math.h>
#include <string.h>

#include "ti_drivers_config.h"
#include "uart_term.h"


#define BIG_STR 100 // larger than any string we could expect to print

///////////////////////////////////////////////////////////////////////////////
///   Debug Definitions
///
/* We will define the first nibble as a the file specifier:
*
*   0000 xxxx => simpleCodeStructure.c
*   0001 xxxx => timerone.c
*   0010 xxxx => timertwo.c
*   0011 xxxx => sensor_queue.c
*   0100 xxxx => sensor_state.c
*   0101 xxxx => uart_term.c
*
*  The second nibble represents the specific location
*/

//in mqtt_client_app.c debug locations
#define DLOC_MT_RECEIVED        0x7F//main thread recevied from RX queue
#define DLOC_MT_SENT_TO_STATS   0x02//sent to state queue in main thread

//in generic_task.c
#define DLOC_GT_START           0x03//started generic task
#define DLOC_GT_WHILE_START     0x04//general Task while loop satrted
#define DLOC_GT_MAKEMSG_START   0x05//general task start making message
#define DLOC_GT_MAKEMSG_TOPIC   0x06//loaded top into msg
#define DLOC_GT_MAKEMSG_TS      0x07//made msg timestamp
#define DLOC_GT_MAKEMSG_SN      0x08//made msg sequence number
#define DLOC_GT_MAKEMSG_PAYLOAD 0x09//Make msg payload
#define DLOC_GT_MAKEMSG_FINISH  0x0A//loaded last of param into msg (finished successfully)
#define DLOC_GT_MSG_PACKED      0x0D//packed MSG in pub task with json lib
#define DLOC_GT_SENT_MSG        0x0E//sent MSG
#define DLOC_GT_ERROR           0x0F//error in pub task after typing to send MSG
#define DLOC_GT_WHILE_EXIT      0x10//exited while loop
#define DLOC_GT_STAT_SEND       0x11 // stats command was sent
#define DLOC_GT_RECEIVED        0x12 //generic task stats command was received


//queues.c debugs
#define DLOC_Q_INIT_SUCC        0x20//successfuly initialized all queues
#define DLOC_Q_SEND_MQ_START    0x25//send to mqtt Queue beginning
#define DLOC_Q_SEND_MQ_SUCC     0x26//successfully sent to mqtt queue
#define DLOC_Q_REC_MQ_START     0x27//receive from mqtt queue starting
#define DLOC_Q_REC_MQ_SUCC      0x28//successfully recevied from mqtt queue
#define DLOC_Q_SEND_STAT_START  0x29//start of send to stats queue
#define DLOC_Q_SEND_STAT_SUCC   0x2A//successfully sent to stats queue
#define DLOC_Q_REC_STAT_START   0x2B//received from stats started
#define DLOC_Q_REC_STAT_SUCC    0x2C//received from stats successful
#define DLOC_Q_SEND_GEN_START   0x2D//started send to general task queue
#define DLOC_Q_SEND_GEN_SUCC    0x2E//successfully sent to general queue
#define DLOC_Q_REC_GEN_START    0x2F//started received from general queue
#define DLOC_Q_REC_GEN_SUCC     0x30//successfully received from general queue

//pubTimer.c debugs
#define DLOC_P_STATS_TIMER_INITD        0x40//successfully init'd stats timer
#define DLOC_P_PUB_TIMER_INITD          0x41//successfully init'd pub timer
#define DLOC_P_STAT_TIMER_CALL_START    0x42//started stats timer callback
#define DLOC_P_STAT_TIMER_CALL_SUCC     0x43//successfully sent msg to stats queue in timer callback
#define DLOC_P_PUB_TIMER_CALL_START     0x44//started pub timer callback
#define DLOC_P_PUB_TIMER_CALL_SUCC      0x45//successfully completed [ub timer callback
#define DLOC_P_STAT_TIMER_CALL_FAIL_STATQ 0x46 //failed to send to stats queue in statsTimerCallback
#define DLOC_P_STAT_TIMER_CALL_FAIL_PUBQ 0x47 //failed to send to pub queue in pubTimerCallback
#define DLOC_P_PUBTIMER_NULL            0x48 //in pubTimer init timer 1 was null
#define DLOC_P_PUBTIMER_STATERR         0x49 //in pubtimer init timer1 start had a status error


//json dbug locations
#define DLOC_J_UNPACK_START          0x50//json start unpack
#define DLOC_J_UNPACK_PARSE_SUCC     0x51//parse successful in unpack
#define DLOC_J_UNPACK_FOR_ENTER      0x52//entered forloop in unpack
#define DLOC_J_UNPACK_FOR_FINISH     0x53//finished forloop in unpack
#define DLOC_J_PACK_START           0x54//started pack
#define DLOC_J_PACK_FINISH      0x56//finished for loop in pack

#define DEBUG_ERROR 123


void dbgOutputLoc(unsigned int outLoc);
void errorHalt(char* location);


void init_gpio();
void intToString(int inInt, char* outStr);
double strToDouble(const char* inStr, int start, int len);
void doubleToStr(double inDouble, char* outStr, int numDec);














/* Define Debug Method */
#define  D_UART_PRINT       0
#define  D_DISPLAY_PRINT    1
#define  F_USER_DEFINED     2

#define D_DEBUG_METHOD    D_UART_PRINT


/* Define Supported Modules */
#define D_GENERAL_IF    (1)
#define D_MQTT_IF       (1)
#define D_OTA_IF        (1)

/* Define Severity Level */
enum
{
    E_TRACE, E_DEBUG, E_INFO, E_WARNING, E_ERROR, E_FATAL, E_DISABLED
};

#define IF_NAME     "GEN"
#define IF_SEVERITY E_INFO


#if (D_DEBUG_METHOD == D_UART_PRINT)
    #include "uart_term.h"
    #define PRINTF(...)  UART_PRINT(__VA_ARGS__);UART_PRINT("\n\r");
#elif (D_DEBUG_METHOD == D_DISPLAY_PRINT)
    #include <ti/display/Display.h>
    extern Display_Handle display;
    #define PRINTF(_module_, _severity_, ...)   Display_printf(display, 0, 0, __VA_ARGS__);
#else
    /* USER DEFIND PRINT METHOD */
    #ifndef PRINTF
        #error "Missing definition of PRINTF method"
    #endif
#endif

#define sl_printf(_severity_, ...) if(_severity_)>= D_SEVERITY)   { PRINTF("[" IF_NAME "] "__VA_ARGS__); }
#define LOG_FATAL( ...)   if(E_FATAL >= IF_SEVERITY)     { PRINTF("[" IF_NAME "::FATAL] "__VA_ARGS__); }
#define LOG_ERROR( ...)   if(E_ERROR >= IF_SEVERITY)     { PRINTF("[" IF_NAME "::ERROR] "__VA_ARGS__); }
#define LOG_WARNING( ...) if(E_WARNING >= IF_SEVERITY)      { PRINTF("[" IF_NAME "::WARN] "__VA_ARGS__); }
#define LOG_INFO( ...)    if(E_INFO >= IF_SEVERITY)      { PRINTF("[" IF_NAME "::INFO] "__VA_ARGS__); }
#define LOG_DEBUG( ...)   if(E_DEBUG >= IF_SEVERITY)     { PRINTF("[" IF_NAME "::DEBUG] "__VA_ARGS__); }
#define LOG_TRACE( ...)   if(E_TRACE >= IF_SEVERITY)     { PRINTF("[" IF_NAME "::TRACE] "__VA_ARGS__); }


#endif // DEBUG_IF_H
