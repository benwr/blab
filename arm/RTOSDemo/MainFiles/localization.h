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
 */

#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "sensor.h"
#include "lcdTask.h"
#include "vtI2C.h"

struct localizationParams
{
    vtI2CStruct * i2cDev;
    vtLCDStruct * lcd;
    xQueueHandle inQueue;
};

/*****************************************************************************/
// Public API

// Start the task
// Args:
//      params: Parameters for the task
//      uxPriority: The priority for the task
//      i2c: Pointer to the I2C task's parameters
//      lcd: Pointer to the parameters for an LCD task (can be NULL)
void startLocalizationTask(struct localizationParams * params,
                           unsigned portBASE_TYPE uxPriority,
                           vtI2CStruct * i2c,
                           vtLCDStruct * lcd);

// Send a timer message to the localization task
// Args:
//      params: The parameters for the localization task
//      ticksElapsed: The number of ticks since the last timer message
//      ticksToBlock: How long the routine should wait if the queue is full
// Return:
//      Result of the call to xQueueSend()
portBASE_TYPE sendLocalizationTimerMessage(struct localizationParams * params,
                                           portTickType ticksElapsed,
                                           portTickType ticksToBlock);

// Send a sensor message to the localization task
// Args:
//      params: The parameters for the localization task
//      sensorValue: The value to send (populated, including a non-TIMER type)
//      ticksToBlock: How long the routine should wait if the queue is full
// Return:
//      Result of the call to xQueueSend()
portBASE_TYPE sendLocalizationSensorMessage(struct localizationParams * params,
                                            struct sensorMessage * sensorValue,
                                            portTickType ticksToBlock);

/*****************************************************************************/
// Private definitions

#endif

