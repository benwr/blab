/* Filename:    configuration.h
 * Description: Configuration definitions for the ARM component of Team 9
 *                  (blab)'s ECE 4534 rover.
 * Author:      Ben Weinstein-Raun <bwr@vt.edu>
 * 
 * Changelog
 * =========
 *  Date        Author
 *  ----        ------
 *  10/01/2014  Ben Weinstein-Raun
 *      Copied some of the values from other files
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"
#include "semphr.h"

/* Communication Limits */
#define I2C_MAX_LEN 14
#define SENSOR_MESSAGE_LEN (I2C_MAX_LEN - 3)
#define LCD_MAX_LEN 40

/* Task stack sizes */
#define BASE_STACK_SIZE 2
#define SENSORIMOTOR_STACK_SIZE ((BASE_STACK_SIZE + 5) * configMINIMAL_STACK_SIZE)
#define LOCALIZATION_STACK_SIZE ((BASE_STACK_SIZE + 5) * configMINIMAL_STACK_SIZE)

/* Task Queue Lengths */
#define LOCALIZATION_QUEUE_LENGTH 10

/* Task priorities */
#define mainQUEUE_POLL_PRIORITY (tskIDLE_PRIORITY)
#define mainSEM_TEST_PRIORITY (tskIDLE_PRIORITY)
#define mainBLOCK_Q_PRIORITY (tskIDLE_PRIORITY)
#define mainUIP_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainINTEGER_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainGEN_QUEUE_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainFLASH_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainLCD_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainI2CTEMP_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainUSB_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainI2CMONITOR_TASK_PRIORITY (tskIDLE_PRIORITY)
#define SENSORIMOTOR_TASK_PRIORITY (tskIDLE_PRIORITY)
 
/* Miscellaneous Values */
#define COMMS_PIC_ADDRESS 0x4f
#define SENSOR_BANKS 3
#define SENSORS_PER_BANK 4
#endif
