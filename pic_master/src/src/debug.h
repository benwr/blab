/* 
 * File:   debug.h
 * Author: alex
 *
 * Created on September 10, 2014, 3:11 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H

//Alex: Comment this to remove debug mode
#define DEBUG_MODE 1


#ifdef DEBUG_MODE
//Alex: Interrupt Codes
//Interrupts modify RB[3:0]
#define DEBUG_I2C_INTERRUPT 0x10



#endif

#endif	/* DEBUG_H */

//Alex: Code to enable outputt on the D register bank
//TRISD0 = 0x00;
//Alex: Code to set all register outputs to high on the D register bank (Assuming thier tris bits are set)
//LATD = 0xff;
