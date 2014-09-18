/* 
 * File:   communication.h
 * Author: alex
 *
 * Created on September 17, 2014, 2:15 PM
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#define PACKET_LENGTH 4

struct packet_type
{
    unsigned char header;
    unsigned char data[ PACKET_LENGTH - 1 ];
    unsigned char checksum_error;
    unsigned char count_error;
};





#endif	/* COMMUNICATION_H */

