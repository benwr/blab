/*
* File: ad.h
* Author: Luke
*
* Created on September 18, 2014, 10:57 AM
*/

#ifndef ADCONFIG_H
#define ADCONFIG_H

/*
typedef struct {
unsigned char distance1;
} SensorData;
*/

void init_registers(void);
//void a_to_d_converter_handler(void);
void a_to_d_converter_handler(void);
char rndSense(unsigned int index);

#endif /* ADCONFIG_H */

#define SENSER_CMD MSGID_SENSOR_STATUS
#define SENSOR_RESPONSE MSGID_SENSOR_RESPOND

#define SENS_CMD_SIZE 14
#define UPDATE_MASK_SIZE 2
#define LB_IR_SENSER 0x0 //Left Back IR sensor.
#define LF_IR_SENSER 0x1 //Left Front IR sensor.
#define RB_IR_SENSER 0x2 //Right Back IR sensor.
#define RF_IR_SENSER 0x3 //Right Front IR sensor.
#define FL_US_SENSER 0x4 //Front Left Ultra Sonic sensor.
#define FR_US_SENSER 0x5 //Front Right Ultra Sonic sensor.

#define MSG_BUF_MASK1 1
#define MSG_BUF_MASK2 2
#define FRONT_LEFT_ULTRA 3
#define FRONT_RIGHT_ULTRA 4
#define LEFT_FRONT_IR 5
#define LEFT_BACK_IR 6
#define RIGHT_FRONT_IR 7
#define RIGHT_BACK_IR 8
#define FRONT_LINE 9
#define BACK_LINE 10
#define RFID_POSSITION 11

//Values to be | woth to set the mask high.
#define FL_ULTRA_UPDATE 0x40
#define FR_ULTRA_UPDATE 0x20
#define LSF_IR_UPDATE 0x10
#define LSB_IR_UPDATE 0x08
#define RSF_IR_UPDATE 0x04
#define RSB_IR_UPDATE 0x02
#define F_LINE_UPDATE 0x01
#define B_Line_UPDATE 0x40
#define RFID_UPDATE 0x20