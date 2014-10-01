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


#define SIDE_CMD 0xa5
#define FRONT_CMD 0xb5
#define VENTRIL_CMD 0xc5
#define SENS_CMD_SIZE 6
