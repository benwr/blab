/*
 * File:   ad.h
 * Author: Luke
 *
 * Created on September 18, 2014, 10:57 AM
 */

#ifndef ADCONFIG_H
#define	ADCONFIG_H


void init_registers(void);
//void a_to_d_converter_handler(void);

#endif	/* ADCONFIG_H */
#ifndef __user_interrupts
#define __user_interrupts
    void a_to_d_converter_handler(void);
#endif

