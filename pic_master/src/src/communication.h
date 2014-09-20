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
#define UART_HEADER_ID_LENGTH 5
#define UART_HEADER_COUNT_LENGTH 3

typedef struct 
{
    unsigned char header;
    unsigned char data[ MESSAGE_LENGTH - 1 ];
    unsigned char bytes_sent;
    unsigned char one_bits_sent;
    unsigned char checksum_error;
    unsigned char count_error;
    unsigned char byte_error;
}uart_packet_type;



void send_uart_message( unsigned char * message_ptr , unsigned char uart_device_id);

void send_uart_header( unsigned char uart_device_id );


#endif	/* COMMUNICATION_H */

