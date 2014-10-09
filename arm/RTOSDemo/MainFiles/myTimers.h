#ifndef _MY_TIMERS_H
#define _MY_TIMERS_H
#include "lcdTask.h"
#include "localization.h"
void startTimerForLCD(vtLCDStruct *vtLCDdata);
void startTimerForTemperature(struct localizationParams * localization);
#endif
