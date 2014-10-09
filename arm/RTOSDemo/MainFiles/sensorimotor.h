/* Filename:    sensorimotor.h
 * Description: Declarations and API for the sensorimotor task on the ARM for
 *                  Team 9 (blab)'s Embedded project. Written for Virginia
 *                  Tech's Embedded Systems Design (ECE 4534) course.
 * Author:      Ben Weinstein-Raun <bwr@vt.edu>
 * 
 * Changelog
 * =========
 *  Date        Author
 *  ----        ------
 *  10/01/2014  Ben Weinstein-Raun
 *      Copied and slightly modified from the "framework" code's 'conductor.h'
 *      file. Mainly style changes and renaming of things.
 */

#ifndef SENSORIMOTOR_H
#define SENSORIMOTOR_H

#include "vtI2C.h"
#include "localization.h"

/*****************************************************************************/
// Public API

// Structure used to pass parameters to the task. Modify only when adding
// or removing parameters.
struct SensorimotorParams
{
    vtI2CStruct * i2cDev;
    struct localizationParams * localization;
};

// Start the task
// Args:
//  params: Parameters for the task
//  uxPriority: The priority of this task
//  i2c: Pointer to an I2C task's parameters
//  localizatoin: pointer to a localization data structure.

void startSensorimotorTask(struct SensorimotorParams * params,
                           unsigned portBASE_TYPE priority,
                           vtI2CStruct * i2c,
                           struct localizationParams * localization);

/*****************************************************************************/
// Private declarations

portTASK_FUNCTION_PROTO(sensorimotorTask, pvParameters);

#endif

