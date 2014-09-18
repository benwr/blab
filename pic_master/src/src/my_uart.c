#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"

//static uart_comm *uc_ptr;

#ifdef __USE18F46J50  //Alex: Begin definitions for the Mk 4

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

    
    //Alex: Initialize send buffer
    uart_send_buffer.current_item = 0;
    uart_send_buffer.last_item = 0;
    uart_send_buffer.buffer[uart_send_buffer.current_item] = 0;
    uart_send_buffer.size = 0;

    //Alex: Initialize receive buffer
    uart_receive_buffer.current_item = 0;
    uart_receive_buffer.last_item = 0;
    uart_receive_buffer.buffer[uart_receive_buffer.current_item] = 0;
    uart_receive_buffer.size = 0;
    
}

//Alex: Put soemthing in the uart send queue
int uart_send_byte( unsigned char sendByte )
{
    if( uart_send_buffer.size >= MAXUARTBUF )
    {
        return 0;        
    }
    else
    {
        uart_send_buffer.buffer[ uart_send_buffer.last_item ] = sendByte;
        uart_send_buffer.last_item = (uart_send_buffer.last_item + 1) % MAXUARTBUF;
        uart_send_buffer.size += 1;
        PIE1bits.TX1IE = 1;
    }

    return 1;
}

//Alex: Remove item from uart send queue and move it into actual hardware transmit buffer; only call in interrupt
void uart_transmit_byte()
{
    if( uart_send_buffer.size <= 0 )
    {
        return;
    }
    TXREG1 = uart_send_buffer.buffer[uart_send_buffer.current_item];
    uart_send_buffer.current_item = (uart_send_buffer.current_item + 1) % MAXUARTBUF;
    uart_send_buffer.size -= 1;

    if( uart_send_buffer.size <= 0 )
    {
        PIE1bits.TX1IE = 0;
    }
}


//Alex: Return 1 if UART send buffer is empty, other wise 0
int uart_send_buffer_empty()
{
    if( uart_send_buffer.size <= 0 )
    {
        uart_send_buffer.size = 0;
        return 1;
    }
    else
    {
        return 0;
    }

}

//Alex: Remove byte from uart receive queue
unsigned char uart_get_byte()
{
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
}



//Alex: Receive a byte into uart receive buffer from the uart hardware recieve buffer; interrupt should call this only
void uart_receive_byte()
{
    if( uart_receive_buffer.size >= MAXUARTBUF )
    {
        PIE1bits.RC1IE = 0; // Turn off interrupts if Receive buffer is full
        return;
    }
    uart_receive_buffer.buffer[uart_receive_buffer.last_item] = RCREG1;
    uart_receive_buffer.last_item = (uart_receive_buffer.last_item + 1) % MAXUARTBUF;
    uart_receive_buffer.size -= 1;
}

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
#endif  //Alex: End definitions for the Mk IV


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


