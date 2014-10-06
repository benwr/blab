/*
 * File:   ad.h
 * Author: Luke
 *
 * Created on September 18, 2014, 10:57 AM
 */

#ifndef ADCONFIG_H
#define	ADCONFIG_H

/*
typedef struct {
    unsigned char distance1;
} SensorData;
*/

void init_registers(void);
//void a_to_d_converter_handler(void);
void a_to_d_converter_handler(void);
char rndSense(unsigned int index);

#endif	/* ADCONFIG_H */


#define SENSER_CMD 0x02
//#define SIDE_CMD 0xa5
//#define FRONT_CMD 0xb5
//#define VENTRIL_CMD 0xc5
#define SENS_CMD_SIZE 14
#define UPDATE_MASK_SIZE 2

//Values to be | woth to set the mask high.
#define FL_ULTRA_UPDATE 0x40
#define FR_ULTRA_UPDATE 0x20
#define LSF_IR_UPDATE 0x10
#define LSR_IR_UPDATE 0x08
#define RSF_IR_UPDATE 0x04
#define RSR_IR_UPDATE 0x02
#define F_LINE_UPDATE 0x01
#define R_Line_UPDATE 0x40
#define RFID_UPDATE 0x20
