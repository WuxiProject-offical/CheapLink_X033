/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "debug.h"

/* doesn't have MTIME */
#define configMTIME_BASE_ADDRESS 	 ( 0 )
#define configMTIMECMP_BASE_ADDRESS  ( 0 )

#define configUSE_PREEMPTION                1
#define configUSE_IDLE_HOOK                 0
#define configUSE_TICK_HOOK                 0
#define configCPU_CLOCK_HZ                  ( ( uint32_t ) ( SystemCoreClock ) )
#define configTICK_RATE_HZ                  ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                ( 15 )
#define configMINIMAL_STACK_SIZE            ( ( uint32_t ) 64 ) /* Can be as low as 60 but some of the demo tasks that use this constant require it to be higher. */
#define configTOTAL_HEAP_SIZE               ( ( size_t ) ( 8 * 1024 ) )
#define configMAX_TASK_NAME_LEN             ( 16 )
#define configUSE_TRACE_FACILITY            0
#define configUSE_16_BIT_TICKS              0
#define configIDLE_SHOULD_YIELD             1
#define configUSE_MUTEXES                   1
#define configQUEUE_REGISTRY_SIZE           8
#define configCHECK_FOR_STACK_OVERFLOW      0
#define configUSE_RECURSIVE_MUTEXES         0
#define configUSE_MALLOC_FAILED_HOOK        0
#define configUSE_APPLICATION_TASK_TAG      0
#define configUSE_COUNTING_SEMAPHORES       1
#define configGENERATE_RUN_TIME_STATS       0

/* Software timer definitions. */
#define configUSE_TIMERS                    0
#define configTIMER_TASK_PRIORITY           ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH            4
#define configTIMER_TASK_STACK_DEPTH        ( configMINIMAL_STACK_SIZE )

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_eTaskGetState               1
#define INCLUDE_xTimerPendFunctionCall      0
#define INCLUDE_xTaskAbortDelay             1
#define INCLUDE_xTaskGetHandle              1
#define INCLUDE_xSemaphoreGetMutexHolder    1

/* Normal assert() semantics without relying on the provision of an assert.h
 * header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); printf("err at line %d of file \"%s\". \r\n ",__LINE__,__FILE__); while(1); }

/* Map to the platform write function. */
#define configPRINT_STRING( pcString )  printf( pcString )
#endif /* FREERTOS_CONFIG_H */
