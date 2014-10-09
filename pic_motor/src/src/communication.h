/* 
 * File:   communication.h
 * Author: alex
 *
 * Created on September 17, 2014, 2:15 PM
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include "my_uart.h"

#define MESSAGE_LENGTH 4

typedef struct 
{    
    unsigned char header;
    unsigned char checksum;
    unsigned char data[ MESSAGE_LENGTH - 2 ];
    unsigned char bytes_sent;
    unsigned char checksum_error;
    unsigned char count_error;
    unsigned char byte_error;
} uart_packet_type;



void send_uart_message( unsigned char * message_ptr );



#endif	/* COMMUNICATION_H */

