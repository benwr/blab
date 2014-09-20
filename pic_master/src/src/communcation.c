#include "communication.h"

static uart_packet_type uart_current_packet;

static unsigned char uart_count;

void send_uart_message( unsigned char * message_ptr , unsigned char uart_device_id )
{
    





}

void send_uart_header( unsigned char uart_device_id )
{
    unsigned char send_byte = ( uart_device_id << 3 ) | ( uart_count );


}
