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

/*
 *  ======== main_freertos.c ========
 */
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include "FreeRTOS.h"
#include "task.h"

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* TI-DRIVERS Header files */
#include "ti_drivers_config.h"

#include "generic_task.h"
#include "stats.h"
#include "debug_if.h"
#include "us_task.h"

extern void * mainThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE   8192


#define GEN_TASK_PRIORITY 2
#define GEN_TASK_STACK_SIZE (2*1024)

#define STATS_TASK_PRIORITY 2
#define STATS_TASK_STACK_SIZE (2*1024)

#define US_TASK_PRIORITY 2
#define US_TASK_STACK_SIZE 4096
/*
 *  ======== main ========
 */
int main(void)
{
    Board_init();
    init_gpio();
    if(InitTerm()==NULL)
        while(1);
    if(init_queue())
        errorHalt("Unable to create queues");


    pthread_t thread;
    pthread_attr_t pAttrs;
    struct sched_param priParam;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 3;
    if(pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED))
        errorHalt(" pthread_attr_setdetachstate() failed");
    pthread_attr_setschedparam(&pAttrs, &priParam);
    if(pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE))
       errorHalt("Unable to set stack size");
    if(pthread_create(&thread, &pAttrs, mainThread, NULL))
        errorHalt("Unable to set create thread");


    pthread_t gen_thread;
    pthread_attr_t gen_pAttrs;
    struct sched_param gen_priParam;

    pthread_attr_init(&gen_pAttrs);
    gen_priParam.sched_priority = GEN_TASK_PRIORITY;
    if(pthread_attr_setdetachstate(&gen_pAttrs, PTHREAD_CREATE_DETACHED))
        errorHalt(" pthread_attr_setdetachstate() failed");
    pthread_attr_setschedparam(&gen_pAttrs, &gen_priParam);
    if(pthread_attr_setstacksize(&gen_pAttrs, GEN_TASK_STACK_SIZE))
        errorHalt("Unable to set stack size");
    if(pthread_create(&gen_thread, &gen_pAttrs, genTask, NULL))
        errorHalt("Unable to create thread");


    pthread_t stats_thread;
    pthread_attr_t stats_pAttrs;
    struct sched_param stats_priParam;

    pthread_attr_init(&stats_pAttrs);
    stats_priParam.sched_priority = STATS_TASK_PRIORITY;
    if(pthread_attr_setdetachstate(&stats_pAttrs, PTHREAD_CREATE_DETACHED))
        errorHalt(" pthread_attr_setdetachstate() failed");
    pthread_attr_setschedparam(&stats_pAttrs, &stats_priParam);
    if(pthread_attr_setstacksize(&stats_pAttrs, STATS_TASK_STACK_SIZE))
        errorHalt("Unable to set stack size");
    if(pthread_create(&stats_thread, &stats_pAttrs, statsTask, NULL))
        errorHalt("Unable to create thread");

    pthread_t us_thread;
    pthread_attr_t us_pAttrs;
    struct sched_param us_priParam;

    pthread_attr_init(&us_pAttrs);
    us_priParam.sched_priority = US_TASK_PRIORITY;
    if(pthread_attr_setdetachstate(&us_pAttrs, PTHREAD_CREATE_DETACHED))
        errorHalt(" pthread_attr_setdetachstate() failed");
    pthread_attr_setschedparam(&us_pAttrs, &us_priParam);
    if(pthread_attr_setstacksize(&us_pAttrs, US_TASK_STACK_SIZE))
        errorHalt("Unable to set stack size");
    if(pthread_create(&us_thread, &us_pAttrs, usTask, NULL))
        errorHalt("Unable to create thread");
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask,
                                   char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}

void vApplicationTickHook(void)
{
    /*
     * This function will be called by each tick interrupt if
     * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
     * added here, but the tick hook is called from an interrupt context, so
     * code must not attempt to block, and only the interrupt safe FreeRTOS API
     * functions can be used (those that end in FromISR()).
     */
}

void vPreSleepProcessing(uint32_t ulExpectedIdleTime)
{
}

//*****************************************************************************
//
//! \brief Application defined idle task hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void
vApplicationIdleHook(void)
{
    /* Handle Idle Hook for Profiling, Power Management etc */
}

//*****************************************************************************
//
//! \brief  Overwrite the GCC _sbrk function which check the heap limit related
//!         to the stack pointer.
//!         In case of freertos this checking will fail.
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
#if defined (__GNUC__)
void * _sbrk(uint32_t delta)
{
    extern char _end;     /* Defined by the linker */
    extern char __HeapLimit;
    static char *heap_end;
    static char *heap_limit;
    char *prev_heap_end;

    if(heap_end == 0)
    {
        heap_end = &_end;
        heap_limit = &__HeapLimit;
    }

    prev_heap_end = heap_end;
    if(prev_heap_end + delta > heap_limit)
    {
        return((void *) -1L);
    }
    heap_end += delta;
    return((void *) prev_heap_end);
}

#endif
