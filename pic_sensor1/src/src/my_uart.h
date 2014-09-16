#ifndef __my_uart_h
#define __my_uart_h

#include "messages.h"
#include "debug.h"

#define MAXUARTBUF 4
#if (MAXUARTBUF > MSGLEN)
#define MAXUARTBUF MSGLEN
#endif

/*
typedef struct __uart_comm
{
    unsigned char buffer[MAXUARTBUF];
    unsigned char buflen;
} uart_comm;
*/

//Buffer to hold uart messages waiting to be transmitted or messages just recieved
struct uart_buffer_type
{
    unsigned char buffer[MAXUARTBUF];
    unsigned char current_item; //Alex: First item waiting to be sent
    unsigned char last_item;    //Alex: Last item waiting to be sent
    char size;
} uart_send_buffer, uart_receive_buffer;


//Alex: Configure UART for transmit and recieve
void uart_configure();

//Alex: Put soemthing in the uart send queue
int uart_send_byte( char sendByte );

//Alex: Remove item from uart send queue and move it into actual hardware transmit buffer; only call in interrupt
void uart_transmit_byte();

//Alex: Return 1 if UART send buffer is empty, other wise 0
int uart_send_buffer_empty();

//Alex: Remove byte from uart receive queue
unsigned char uart_get_byte();

//Alex: Receive a byte from the uart hardware recieve buffer; interrupt should call this only
void uart_receive_byte();

//Alex: Return 1 if UART recieve buffer is empty, other wise 0
int uart_receive_buffer_empty();

//void init_uart_recv(uart_comm *);
//void uart_recv_int_handler(void);

#endif
