/* Filename:    localization.h
 * Description: Declarations and API for the localization task on the ARM for
 *                  Team 9 (blab)'s ECE 4534 rover.
 * Author:      Ben Weinstein-Raun <bwr@vt.edu>
 * 
 * Changelog
 * =========
 *  Date        Author
 *  ----        ------
 *  10/01/2014  Ben Weinstein-Raun
 *      Copied and modified i2cTemp.h from the "framework" code, making
 *      primarily stylistic and naming changes.
 *
 *  10/08/2014  Ben Weinstein-Raun
 *      Sidestepped an issue with printing "too much data" in one line by simply
 *      not printing so much data. Also updated message types to represent the
 *      fact that we are putting all sensor data in one "STATUS" message, rather
 *      than splitting it into FRONT, SIDES, and VENTRAL messages.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS  includes */
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"
#include "semphr.h"

/* Project-specific includes */
#include "vtUtilities.h"
#include "vtI2C.h"
#include "I2CTaskMsgTypes.h"
#include "LCDtask.h"
#include "localization.h"


static portTASK_FUNCTION_PROTO(localizationTask, pvParameters);


// Public API

void
startLocalizationTask(struct localizationParams * params,
                      unsigned portBASE_TYPE uxPriority,
                      vtI2CStruct * i2c,
                      vtLCDStruct * lcd)
{
    // Create the queue for this task
    if ((params->inQueue = xQueueCreate(LOCALIZATION_QUEUE_LENGTH, 
                                        sizeof(struct sensorMessage))) == NULL) {
        VT_HANDLE_FATAL_ERROR(0);
    }

    portBASE_TYPE retval;
    params->i2cDev = i2c;
    params->lcd = lcd;
    if ((retval = xTaskCreate(localizationTask,
                              (signed char *) "Localization",
                              LOCALIZATION_STACK_SIZE,
                              (void *) params,
                              uxPriority,
                              (xTaskHandle *) NULL)) != pdPASS) {
        VT_HANDLE_FATAL_ERROR(retval);
    }
}

portBASE_TYPE
sendLocalizationTimerMessage(struct localizationParams * params,
                             portTickType ticksElapsed,
                             portTickType ticksToBlock)
{
    if (params == NULL) {
        VT_HANDLE_FATAL_ERROR(0);
    }

    struct sensorMessage messageBuffer;
    messageBuffer.type = TIMER;

    if (sizeof(ticksElapsed) > SENSOR_MESSAGE_LEN) {
        // No room for the timer message
        VT_HANDLE_FATAL_ERROR(sizeof(ticksElapsed));
    }
    memcpy(messageBuffer.values,
           (uint8_t *) &ticksElapsed, sizeof(ticksElapsed));

    return (xQueueSend(params->inQueue,
                       (void *) (&messageBuffer),
                       ticksToBlock));
}

portBASE_TYPE
sendLocalizationSensorMessage(struct localizationParams * params,
                              struct sensorMessage * sensorValue,
                              portTickType ticksToBlock)
{
    if (params == NULL) {
        VT_HANDLE_FATAL_ERROR(0);
    }

    return (xQueueSend(params->inQueue, (void *) sensorValue, ticksToBlock));
}

// End of public API

static inline enum sensorType 
getSensorType(struct sensorMessage * message)
{
    return message->type;
}

static inline uint8_t
getValue(struct sensorMessage * message, uint8_t index)
{
    if (index >= SENSOR_MESSAGE_LEN) {
        VT_HANDLE_FATAL_ERROR(index);
    }

    return message->values[index];
}

static portTASK_FUNCTION(localizationTask, pvParameters)
{
    struct localizationParams * params
        = (struct localizationParams *) pvParameters;
    vtI2CStruct * i2c = params->i2cDev;
    vtLCDStruct * lcd = params->lcd;
    char lcdBuffer[LCD_MAX_LEN + 1];


    struct sensorMessage messageBuffer;

    /*
    uint8_t FRONTVALUES [SENSOR_MESSAGE_LEN];
    uint8_t SIDEVALUES [SENSOR_MESSAGE_LEN];
    uint8_t VENTRALVALUES [SENSOR_MESSAGE_LEN];
    */

    enum sensorType MESSAGETYPES [2] = {TIMER, STATUS};

    uint8_t STATUS_MESSAGE [I2C_MAX_LEN]
        = {0x02, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};


    for (;;) {
        if (xQueueReceive(params->inQueue, 
                          (void *) &messageBuffer, 
                          portMAX_DELAY)
            != pdTRUE) {
            VT_HANDLE_FATAL_ERROR(0);
        }

        // Based on the message type, we decide on an action to take
        uint8_t lcdValue[1] = {getValue(&messageBuffer, 0)};

        int i;
        switch (messageBuffer.type) {
        case TIMER:
            if (vtI2CEnQ(i2c,
                         STATUS,
                         COMMS_PIC_ADDRESS,
                         SENSOR_MESSAGE_LEN,
                         STATUS_MESSAGE,
                         SENSOR_MESSAGE_LEN)
                != pdTRUE) {
                VT_HANDLE_FATAL_ERROR(0);
            }
            break;
        case STATUS:
            if (messageBuffer.updateMask)
            {
                sprintf(lcdBuffer, 
                        "%03ucm        ", 
                        (unsigned int) getValue(&messageBuffer, 0));
            } else {
                sprintf(lcdBuffer, "Stale data");
            }

            if (lcd != NULL) {
                if (SendLCDPointMsg(lcd,
                                    1,
                                    lcdValue,
                                    portMAX_DELAY) != pdTRUE) {
                    VT_HANDLE_FATAL_ERROR(0);
                }
                if (SendLCDPrintMsg(lcd,
                                    strnlen(lcdBuffer, LCD_MAX_LEN),
                                    lcdBuffer,
                                    portMAX_DELAY) != pdTRUE) {
                    VT_HANDLE_FATAL_ERROR(0);
                }
            }
            break;
        default:
            VT_HANDLE_FATAL_ERROR(0);
            break;
        }
    }
}

