/* 
 * File:   debug.h
 * Author: alex
 *
 * Created on September 10, 2014, 3:11 PM
 */
#include <stdio.h>
#include "maindefs.h"

#ifndef DEBUG_H
#define	DEBUG_H

//Alex: Comment this to remove debug mode
#define DEBUG_MODE 1


#ifdef DEBUG_MODE
//Alex: Interrupt Codes
//Interrupts modify RD
#define DEBUG_NONE                      0x00
#define DEBUG_I2C_INTERRUPT             0x01
#define DEBUG_I2C_IDLE                  0x02


#define DEBUG_UART_RECEIVE_INTERRUPT    0x04

#define DEBUG_UART_SEND_INTERRUPT       0x05
#define DEBUG_UART_MSGQ_SEND_TO_TX      0x06
#define DEBUG_UART_MSGQ_RECV_FROM_MAIN  0x07

#define DEBUG_TIMER0_INTERRUPT          0x08
#define DEBUG_TIMER1_INTERRUPT          0x09


void debug_configure();
void blip();
void blip1();
void blip2();
void blip3();
void blip4();
void blink0();
void blink1();


#endif



#endif	/* DEBUG_H */

//Alex: Code to enable outputt on the D register bank
//TRISD0 = 0x00;
//Alex: Code to set all register outputs to high on the D register bank (Assuming thier tris bits are set)
//LATD = 0xff;
