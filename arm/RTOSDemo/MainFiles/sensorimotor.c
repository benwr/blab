/* Filename:    sensorimotor.c
 * Description: Definitions and implementation for the sensorimotor task on the
 *                  ARM for Team 9 (blab)'s Embedded project. Written for 
 *                  Virginia Tech's Embedded Systems Design (ECE 4534) course.
 * Author:      Ben Weinstein-Raun <bwr@vt.edu>
 * 
 * Changelog
 * =========
 *  Date        Author
 *  ----        ------
 *  10/01/2014  Ben Weinstein-Raun
 *      Copied and slightly modified from the "framework" code's 'conductor.c'
 *      file. Mainly style changes and renaming of things.
 *
 *  10/08/2014  Ben Weinstein-Raun
 *      Modified the code that reads in status messages to make it conform to
 *      our new format.
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"
#include "semphr.h"

/* Project-specific includes */
#include "configuration.h"
#include "vtUtilities.h"
#include "vtI2C.h"
#include "localization.h"
#include "I2CTaskMsgTypes.h"
#include "sensorimotor.h"

void startSensorimotorTask(struct SensorimotorParams * params,
                           unsigned portBASE_TYPE uxPriority,
                           vtI2CStruct * i2c,
                           struct localizationParams * localization)
{
    portBASE_TYPE retval;
    params->i2cDev = i2c;
    params->localization = localization;
    if ((retval = xTaskCreate(sensorimotorTask,
                              (signed char *) "Sensorimotor",
                              SENSORIMOTOR_STACK_SIZE,
                              (void *) params,
                              uxPriority,
                              (xTaskHandle *) NULL)) != pdPASS) {
        VT_HANDLE_FATAL_ERROR(retval);
    }
}

portTASK_FUNCTION(sensorimotorTask, pvParameters)
{
    uint8_t rxLen, status;
    uint8_t buffer[I2C_MAX_LEN];

    struct SensorimotorParams * params = (struct SensorimotorParams *) pvParameters;
    vtI2CStruct * i2cDev = params->i2cDev;

    // LCD Information Pointer
    struct localizationParams * localization = params->localization;
    enum sensorType receivedMessageType;

    for (;;) {
        if (vtI2CDeQ(i2cDev, 
                     I2C_MAX_LEN, 
                     buffer, 
                     &rxLen, 
                     &receivedMessageType, 
                     &status) 
            != pdTRUE) {
            VT_HANDLE_FATAL_ERROR(0);
        }
            
        struct sensorMessage message;
        message.messageID = buffer[0];
        message.type = receivedMessageType;
        memcpy(message.values, &buffer[3], SENSOR_MESSAGE_LEN);
        message.updateMask = (buffer[1] << 8) | (buffer[2] << 8);

        switch(receivedMessageType) {
        case STATUS:
            sendLocalizationSensorMessage(localization, &message, portMAX_DELAY);
            break;
        default:
            VT_HANDLE_FATAL_ERROR(receivedMessageType);
            break;
        }
    }
}
