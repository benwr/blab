#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"
#include "debug.h"





#define UART_DATA_LENGTH MSGLEN
#define UART_FRAME_LENGTH (UART_DATA_LENGTH + UART_HEADER_WIDTH + UART_FOOTER_WIDTH)
#define MAXUARTBUF UART_FRAME_LENGTH

//UART Header Information
#define UART_HEADER_WIDTH 2
#define UART_HEADER_BYTE 0xff
#define UART_POS_HEADER_BYTE 0
#define UART_POS_COUNTER 1

//UART Footer Information
#define UART_FOOTER_WIDTH 2
#define UART_FOOTER_BYTE 0xfe
#define UART_POS_FOOTER_BYTE UART_FRAME_LENGTH - 1
#define UART_POS_CHECKSUM UART_FRAME_LENGTH - 2

//Buffer to hold uart messages waiting to be transmitted or messages just recieved
typedef struct
{
    unsigned char data[MAXUARTBUF];
    //unsigned char current_item; //Alex: First item waiting to be sent
    //unsigned char last_item;    //Alex: Last item waiting to be sent
    unsigned char index;
    unsigned char size;
    unsigned char done;
} uart_buffer_type;

typedef struct
{
    unsigned char header;
    unsigned char checksum;
    unsigned char data[ UART_FRAME_LENGTH - 2 ];
    //const unsigned char data_length = UART_FRAME_LENGTH - 2;
    unsigned char bytes_received;
    unsigned char checksum_error;
    unsigned char count_error;
    unsigned char byte_error;
    unsigned char acked;
} uart_packet_type;


//Definition for send_uart_message
#define SEND_UART_MESSAGE_ALL_GOOD      0x00
#define SEND_UART_MESSAGE_BAD_LENGTH    0x01
#define SEND_UART_MESSAGE_Q_FULL        0x02
//Send a UART Packet
unsigned char send_uart_message( unsigned char * message_ptr );

//Alex: Configure UART for transmit and recieve
void uart_configure( );

//Handle receive interrupt
void uart_receive_interrupt_handler();

//Handle uart transmit handler
void uart_transmit_interrupt_handler();

//Alex: Return 1 if UART send buffer is empty, other wise 0
//int uart_send_buffer_empty();


//Alex: Receive a byte from the uart hardware recieve buffer; interrupt should call this only
//void uart_receive_byte();

//Alex: Return 1 if UART recieve buffer is empty, other wise 0
//int uart_receive_buffer_empty();

//return number of bytes in receive buffer
unsigned char uart_num_bytes_in_recv_buffer();

//void init_uart_recv(uart_comm *);
//void uart_recv_int_handler(void);

void uart_frame_message( unsigned char * inner , unsigned char * outer );

#endif
