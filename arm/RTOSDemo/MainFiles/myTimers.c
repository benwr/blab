/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"
#include "timers.h"

/* include files. */
#include "vtUtilities.h"
#include "LCDtask.h"
#include "myTimers.h"

/* **************************************************************** */
// WARNING: Do not print in this file -- the stack is not large enough for this
// task
/* **************************************************************** */

/* *********************************************************** */
// Functions for the LCD Task related timer
//
// how often the timer that sends messages to the LCD task should run
// Set the task up to run every 100 ms
#define lcdWRITE_RATE_BASE ((portTickType)500 / portTICK_RATE_MS)
#define tempI2C_RATE 160  // In terms of reads / 8 seconds

// Callback function that is called by the LCDTimer
//   Sends a message to the queue that is read by the LCD Task
void LCDTimerCallback(xTimerHandle pxTimer)
{
  if (pxTimer == NULL) {
    VT_HANDLE_FATAL_ERROR(0);
  } else {
    // When setting up this timer, I put the pointer to the
    //   LCD structure as the "timer ID" so that I could access
    //   that structure here -- which I need to do to get the
    //   address of the message queue to send to
    vtLCDStruct *ptr = (vtLCDStruct *)pvTimerGetTimerID(pxTimer);
    // Make this non-blocking *but* be aware that if the queue is full, this
    // routine
    // will not care, so if you care, you need to check something
    if (SendLCDTimerMsg(ptr, lcdWRITE_RATE_BASE, 0) == errQUEUE_FULL) {
      // Here is where you would do something if you wanted to handle the queue
      // being full
      //VT_HANDLE_FATAL_ERROR(0);
    }
  }
}

void startTimerForLCD(vtLCDStruct *vtLCDdata)
{
  if (sizeof(long) != sizeof(vtLCDStruct *)) {
    VT_HANDLE_FATAL_ERROR(0);
  }
  xTimerHandle LCDTimerHandle = xTimerCreate((const signed char *)"LCD Timer",
                                             lcdWRITE_RATE_BASE,
                                             pdTRUE,
                                             (void *)vtLCDdata,
                                             LCDTimerCallback);
  if (LCDTimerHandle == NULL) {
    VT_HANDLE_FATAL_ERROR(0);
  } else {
    if (xTimerStart(LCDTimerHandle, 0) != pdPASS) {
      VT_HANDLE_FATAL_ERROR(0);
    }
  }
}

/* *********************************************************** */
// Functions for the Temperature Task related timer
//
// how often the timer that sends messages to the LCD task should run
// Set the task up to run every 500 ms
#define tempWRITE_RATE_BASE ((portTickType)500 / portTICK_RATE_MS)
// Callback function that is called by the TemperatureTimer
//   Sends a message to the queue that is read by the Temperature Task
void TempTimerCallback(xTimerHandle pxTimer)
{
  if (pxTimer == NULL) {
    VT_HANDLE_FATAL_ERROR(0);
  } else {
    // When setting up this timer, I put the pointer to the
    //   Temperature structure as the "timer ID" so that I could access
    //   that structure here -- which I need to do to get the
    //   address of the message queue to send to
    struct localizationParams *ptr = (struct localizationParams *)pvTimerGetTimerID(pxTimer);
    // Make this non-blocking *but* be aware that if the queue is full, this
    // routine
    // will not care, so if you care, you need to check something
    if (sendLocalizationTimerMessage(ptr, tempWRITE_RATE_BASE, 0) == errQUEUE_FULL) {
      // Here is where you would do something if you wanted to handle the queue
      // being full
      //VT_HANDLE_FATAL_ERROR(0);
    }
  }
}

void startTimerForTemperature(struct localizationParams * localization)
{
  if (sizeof(long) != sizeof(struct localizationParams *)) {
    VT_HANDLE_FATAL_ERROR(0);
  }
  xTimerHandle TempTimerHandle =
      xTimerCreate((const signed char *) "Localization Timer",
                   16 * tempWRITE_RATE_BASE / tempI2C_RATE,
                   pdTRUE,
                   (void *) localization,
                   TempTimerCallback);
  if (TempTimerHandle == NULL) {
    VT_HANDLE_FATAL_ERROR(0);
  } else {
    if (xTimerStart(TempTimerHandle, 0) != pdPASS) {
      VT_HANDLE_FATAL_ERROR(0);
    }
  }
}
