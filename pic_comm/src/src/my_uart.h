#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"
#include "debug.h"

#define MAXUARTBUF 4
#define MESSAGE_LENGTH 4

//Buffer to hold uart messages waiting to be transmitted or messages just recieved
typedef struct
{
    unsigned char buffer[MAXUARTBUF];
    unsigned char current_item; //Alex: First item waiting to be sent
    unsigned char last_item;    //Alex: Last item waiting to be sent
    char size;
} uart_buffer_type;

typedef struct
{
    unsigned char header;
    unsigned char checksum;
    unsigned char data[ MESSAGE_LENGTH - 2 ];
    //const unsigned char data_length = MESSAGE_LENGTH - 2;
    unsigned char bytes_received;
    unsigned char checksum_error;
    unsigned char count_error;
    unsigned char byte_error;
} uart_packet_type;


//Send a UART Packet
void send_uart_message( unsigned char * message_ptr );

//Receive a UART Packet
unsigned char receive_uart_message( unsigned char * message_ptr );

//Alex: Configure UART for transmit and recieve
void uart_configure( );

//Alex: Put soemthing in the uart send queue
int uart_send_byte( char sendByte );

//Alex: Remove item from uart send queue and move it into actual hardware transmit buffer; only call in interrupt
void uart_transmit_byte();

//Alex: Return 1 if UART send buffer is empty, other wise 0
//int uart_send_buffer_empty();

//Alex: Remove byte from uart receive queue
unsigned char uart_get_byte();

//Alex: Receive a byte from the uart hardware recieve buffer; interrupt should call this only
void uart_receive_byte();

//Alex: Return 1 if UART recieve buffer is empty, other wise 0
//int uart_receive_buffer_empty();

//return number of bytes in receive buffer
unsigned char uart_num_bytes_in_recv_buffer();

//void init_uart_recv(uart_comm *);
//void uart_recv_int_handler(void);

#endif
