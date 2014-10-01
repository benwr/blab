#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"
#include "messages.h"

//static uart_comm *uc_ptr;

#ifdef __USE18F46J50  //Alex: Begin definitions for the Mk 4
//static uart_packet_type * m_uart_send_buffer;
//static uart_buffer_type * m_uart_receive_buffer;

//static unsigned char uart_sent_count;

//static unsigned char receive_counter;

//Send a UART Packet
unsigned char send_uart_message( unsigned char length , unsigned char * message_ptr )
{
    static unsigned char sent_counter = 0;

    if( (length + 2) > UART_MESSAGE_LENGTH )
    {
        return SEND_UART_MESSAGE_BAD_LENGTH;
    }

    unsigned char uart_message[UART_MESSAGE_LENGTH];

    //Send header as counter value and increment counter
    uart_message[0] = sent_counter++;

    //Send data and calculate checksum
    unsigned char checksum = 0x00;
    int i;
    for(i=0;i<(length);i++)
    {
        uart_message[i+1] = message_ptr[i];
        checksum = checksum ^ message_ptr[i];
    }
    i++;

    //Send checksum as footer
    uart_message[i] = checksum;

    unsigned char message_q_code = FromMainLow_sendmsg((length+2),MSGT_UART_DATA,(void *) uart_message );

    if( message_q_code == MSGQUEUE_FULL)
    {
        return SEND_UART_MESSAGE_Q_FULL;
    }


    

    PIE1bits.TX1IE = 1;
    return SEND_UART_MESSAGE_ALL_GOOD;
}

//Receive a UART Packet
unsigned char receive_uart_message(  )
{
    static uart_packet_type uart_recv_packet;

    unsigned char gotten_byte = RCREG1;

    if( uart_recv_packet.bytes_received < (UART_MESSAGE_LENGTH) )
    {
        uart_recv_packet.data[ uart_recv_packet.bytes_received - 1 ];
        uart_recv_packet.bytes_received += 1;
    }
    else
    {
        //signed char err =  ToMainLow_sendmsg(unsigned char,unsigned char,void *);

    }

    return 0x00; //change this
}

//Alex: Configure UART for transmit
void uart_configure()
{   
    TRISCbits.TRISC7 = 1;   //Alex: Enable Tx pin for output
    TRISCbits.TRISC6 = 0;   //Alex: Enable Rx pin for input

    BAUDCONbits.BRG16 = 1;  //Alex: Set the 16-bit baud rate generator
    TXSTA1bits.BRGH = 1;   //Alex: Set the high resolution baud rate

    //Alex: Set Baud rate speed generator thingys
    SPBRGH1 = 0x00;
    SPBRG1 = 0xCF;

    TXSTA1bits.SYNC = 0; //Alex: Set Asynchronous mode
    RCSTA1bits.SPEN = 1; //Alex: Enable UART module 1..or something

    // Alex: USART TX interrupt priority
    IPR1bits.TX1IP = 0;

    PIE1bits.TX1IE = 0; //Alex: Set Transmit Interrupt on
    PIE1bits.RC1IE = 1; //Alex: Set Receive Interrupt on

    TXSTA1bits.TXEN = 1; //Alex: Enable Transmission
    RCSTA1bits.CREN = 1; //Alex: Enable Reception  
   
    
}


#endif  //Alex: End definitions for the Mk IV
//Handle receive interrupt
void uart_receive_interrupt_handler()
{
    //static unsigned char done = 1;
    static unsigned char * data;
    static unsigned char index = 0;
    static unsigned char new = 1;
    static unsigned char received_counter = 0;


    if( index < MAXUARTBUF )
    {
        data[index] = RCREG1;
        index++;
        new = 0;
    }
    else
    {
        received_counter++;
        if( received_counter != data[0] )
        {
            return;
            //error handle this!
        }

        if( (data[1] ^ data[2] ^ data[3] ^ data[4] ^ data[5] ^ data[6]) != data[7]  )
        {
            return;
            //error handle this!
        }

        //send ack?

        //Fill data for sending to main
        unsigned char gooey_uart_center[UART_MESSAGE_LENGTH];

        int i;
        for(i = 0; i < UART_MESSAGE_LENGTH; i++)
        {
            gooey_uart_center[i] = data[i+1];
        }

        unsigned char status = ToMainLow_sendmsg(UART_MESSAGE_LENGTH,MSGT_UART_DATA,(void *) gooey_uart_center );
        if( status == MSGQUEUE_FULL )
        {
            //Oh shit
        }
        else
        {
            new = 1;
            index = 0;
        }
    }


    
   

}

//Handle uart transmit handler
void uart_transmit_interrupt_handler()
{
    static unsigned char done = 1;
    static unsigned char * data;
    static unsigned char index = 0;

    unsigned char msgtype;;
    if( done )
    {        
        signed char message_status =  FromMainLow_recvmsg(4,&msgtype, (void*)data );
        if( message_status == MSGQUEUE_EMPTY)
        {            
            PIE1bits.TX1IE = 0;
        }
        else if( message_status > 0 )
        {
            index = 0;
            done = 0;  
            
            TXREG1 = data[index];
            index++;
            if( index >= UART_MESSAGE_LENGTH )
            {
                done = 1;
            }            
        }
        else if( message_status == MSGQUEUE_FULL)
        {
            //blip();
        }
        else if( message_status == MSGBAD_LEN )
        {
            //blip();
        }
        else if( message_status == MSGBUFFER_TOOSMALL )
        {
            //blip();
        }
    }
    else
    {
        TXREG1 = data[index];
        index++;
        if( index >= UART_MESSAGE_LENGTH )
        {
            done = 1;
        }
    }
    

}


//Alex: Remove byte from uart receive queue
unsigned char uart_get_byte()
{
    unsigned char gotten_byte = 0;
    char msgtype = MSGT_UART_DATA;
    signed char message_status = ToMainLow_recvmsg(1,&msgtype,(void *) &gotten_byte );

    return gotten_byte;

    /*
    if( uart_receive_buffer.size <= 0 )
    {
        uart_receive_buffer.size = 0;
    }
    unsigned char return_value = uart_receive_buffer.buffer[uart_receive_buffer.current_item];
    uart_receive_buffer.current_item = ( uart_receive_buffer.current_item + 1 ) % MAXUARTBUF;
    uart_receive_buffer.size -= 1;

    if( PIE1bits.RC1IE == 0 )   // Make sure interrupts ready to recieve
    {
        PIE1bits.RC1IE = 1;
    }

    return return_value;
    */
}



//Alex: Receive a byte into uart receive buffer from the uart hardware recieve buffer; interrupt should call this only
/*
void uart_receive_byte()
{
    
    signed char message_status = ToMainLow_sendmsg(1,MSGT_UART_DATA,(void *) &gotten_byte );

    
    /*
    if( uart_receive_buffer.size >= MAXUARTBUF )
    {
        PIE1bits.RC1IE = 0; // Turn off interrupts if Receive buffer is full
        return;
    }
    uart_receive_buffer.buffer[uart_receive_buffer.last_item] = RCREG1;
    uart_receive_buffer.last_item = (uart_receive_buffer.last_item + 1) % MAXUARTBUF;
    uart_receive_buffer.size += 1;
     
}
*/

/*
int uart_receive_buffer_empty()
{
    if( uart_receive_buffer.size == 0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
 * */

/*
unsigned char uart_num_bytes_in_recv_buffer()
{
    return uart_receive_buffer.size;
}
*/



/*
void uart_recv_int_handler() {

#ifdef DEBUG_MODE
    //Alex: Set Debug output
    LATD = DEBUG_UARTREC_INTERRUPT;
#endif

#ifdef __USE18F26J50
    if (DataRdy1USART()) {
        uc_ptr->buffer[uc_ptr->buflen] = Read1USART();
#else
#ifdef __USE18F46J50
    if (DataRdy1USART()) {
        uc_ptr->buffer[uc_ptr->buflen] = Read1USART();
#else
    if (DataRdyUSART()) {
        uc_ptr->buffer[uc_ptr->buflen] = ReadUSART();
#endif
#endif

        uc_ptr->buflen++;
        // check if a message should be sent
        if (uc_ptr->buflen == MAXUARTBUF) {
            ToMainLow_sendmsg(uc_ptr->buflen, MSGT_UART_DATA, (void *) uc_ptr->buffer);
            uc_ptr->buflen = 0;
        }
    }
#ifdef __USE18F26J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
#ifdef __USE18F46J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
    if (USART_Status.OVERRUN_ERROR == 1) {
#endif
#endif
        // we've overrun the USART and must reset
        // send an error message for this
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        ToMainLow_sendmsg(0, MSGT_OVERRUN, (void *) 0);
    }

    #ifdef DEBUG_MODE
    //Alex: Release Debug Output
    LATD = DEBUG_NONE;

    #endif
}
*/

/*
void init_uart_recv(uart_comm *uc) {
    uc_ptr = uc;
    uc_ptr->buflen = 0;
}
*/


