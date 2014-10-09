/* Filename:    sensor.h
 * Description: Sensor-related definitions
 * Author:      Ben Weinstein-Raun <bwr@vt.edu>
 * 
 * Changelog
 * =========
 *  Date        Author
 *  ----        ------
 *  10/01/2014  Ben Weinstein-Raun
 *      Wrote simple sensorMessage type
 *
 *  10/08/2014  Ben Weinstein-Raun
 *      Removed multiple message types for different sensors; we're just reading
 *      all of them in at once in a STATUS message.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "configuration.h"

enum sensorType {
    TIMER,
    STATUS,
};

struct sensorMessage
{
    enum sensorType type;
    uint8_t messageID;
    uint16_t updateMask;
    uint8_t values[SENSOR_MESSAGE_LEN];
};

#endif
