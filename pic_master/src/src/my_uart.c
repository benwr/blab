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
    static unsigned char done = 0;
    static unsigned char received_counter = 0;
    static unsigned char checksum = 0;
    static unsigned char start_byte_bad = 0;

    if(!done)
    {
        data[index] = RCREG1;
        if( index == 0 )
        {
            if( data[0] != UART_HEADER_BYTE )   // No 0xff as start byte
            {
                if( !start_byte_bad )
                {
                    start_byte_bad = 1;
                    unsigned char bad_start_id = received_counter;
                    ToMainLow_sendmsg(1,MSGT_UART_BAD_START,(void *)&bad_start_id);
                }
                index = 0;
                return;
            }
            else
            {
                start_byte_bad = 0;
            }
        }
        else if( ( index >= UART_HEADER_WIDTH ) && ( index < UART_DATA_LENGTH + UART_HEADER_WIDTH )  )
        {
            checksum ^= data[index];
        }
        else if( index >= UART_POS_FOOTER_BYTE )
        {
            if( data[UART_POS_FOOTER_BYTE] != UART_FOOTER_BYTE )    //No 0xfe as end byte
            {
                //Reset index because message needs to restart
                index = 0;

                //Send Error message to main to be sent to sender
                unsigned char bad_end_id = received_counter;
                ToMainLow_sendmsg(1,MSGT_UART_BAD_END,(void *)&bad_end_id);

                return;
            }
        }
        index++;
    }
    
    if( index >= UART_FRAME_LENGTH )
    {


        index = 0;
        received_counter++;

        if( received_counter != data[UART_POS_COUNTER] )
        {
            //Send Error message to main to be sent to sender
            unsigned char bad_counter_id[2];
            bad_counter_id[0] = received_counter;
            bad_counter_id[1] = data[UART_POS_COUNTER];   //Format for bad counter in error message sent
            ToMainLow_sendmsg(2,MSGT_UART_BAD_COUNTER,(void *)&bad_counter_id);
        }

        if( checksum != data[UART_FRAME_LENGTH - UART_FOOTER_WIDTH] )
        {
            //Send Error message to main to be sent to sender
            unsigned char bad_checksum_id = received_counter;
            ToMainLow_sendmsg(1,MSGT_UART_BAD_CHECKSUM,(void *)&bad_checksum_id);
        }

        //send ack?

        //Fill data for sending to main
        unsigned char gooey_uart_center[UART_DATA_LENGTH];

        int i;
        for(i = 0; i < UART_DATA_LENGTH; i++)
        {
            gooey_uart_center[i] = data[i+UART_HEADER_WIDTH];
        }

        unsigned char status = ToMainLow_sendmsg(UART_FRAME_LENGTH,MSGT_UART_DATA,(void *) gooey_uart_center );
        if( status == MSGQUEUE_FULL )
        {
            //Oh shit
        }
        else
        {
            
            
        }
    }


    
   

}


//Send a UART Packet
unsigned char send_uart_message( unsigned char * message_ptr )
{
    unsigned char message_q_code = FromMainLow_sendmsg(UART_DATA_LENGTH,MSGT_UART_DATA,(void *) message_ptr );

    if( message_q_code == MSGQUEUE_FULL)
    {
        return SEND_UART_MESSAGE_Q_FULL;
    }
     
    PIE1bits.TX1IE = 1;
    return SEND_UART_MESSAGE_ALL_GOOD;
}

//Handle uart transmit handler
void uart_transmit_interrupt_handler()
{
    blip1();
    static unsigned char done = 1;    
    static unsigned char index = 0;
    static unsigned char sent_counter = 0;

    unsigned char * data;
    static unsigned char * message;

    unsigned char msgtype = MSGT_UART_DATA;

    if( done )
    {
        blip2();
        signed char message_status =  FromMainLow_recvmsg(UART_DATA_LENGTH,&msgtype, (void*)data );
        if( message_status == MSGQUEUE_EMPTY)
        {
            blip();
            PIE1bits.TX1IE = 0;
        }
        else if( message_status > 0 )
        {
            blip3();
            message = uart_frame_message( data );
            index = 0;
            done = 0;  
            
            TXREG1 = message[index];
            index++;
            if( index >= UART_FRAME_LENGTH )
            {
                done = 1;
            }            
        }
        else if( message_status == MSGQUEUE_FULL)
        {
            
        }
        else if( message_status == MSGBAD_LEN )
        {
            
        }
        else if( message_status == MSGBUFFER_TOOSMALL )
        {
            
        }
    }
    else
    {
        blip4();
        TXREG1 = message[index];
        index++;
        if( index >= UART_FRAME_LENGTH )
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


unsigned char * uart_frame_message( unsigned char * inner )
{
    static unsigned char sent_counter = 0;

    static unsigned char outer[UART_FRAME_LENGTH];    //Actual message to be sent over UART

    outer[0] = 0xff;                    //Insert start byte
    outer[1] = sent_counter++;          //Insert counter value
    outer[UART_FRAME_LENGTH-1] = 0xfe;  //Insert end byte

    unsigned char checksum = 0;
    unsigned char i;
    for(i=0;i<UART_DATA_LENGTH;i++)
    {
        if( inner[i] == 0xff || inner[i] == 0xfe )  //Check for sigil values
        {
            outer[i+2] = 0xfd;                        //"scale" back these values
        }
        else
        {
            outer[i+2] = inner[i];                      //Fill outer
        }
        checksum ^= inner[i];                       //calculate checksum
    }
    outer[UART_FRAME_LENGTH-2] = checksum;

    return outer;
}