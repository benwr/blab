#include "communication.h"
#include "my_uart.h"

static uart_packet_type uart_current_packet;

//static unsigned char uart_sent_count;
static unsigned char sent_counter;


/*
void send_uart_message( unsigned char * message_ptr )
{
    //Set header equal to counter value
    uart_current_packet.header = sent_counter;
    sent_counter++;

    //Calculate Checksum
    int i;
    for(i=0;i<(MESSAGE_LENGTH - 2);i++)
    {
        uart_current_packet.checksum = uart_current_packet.checksum ^ uart_current_packet.data[i];
    }
    

}

*/