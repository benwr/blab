#include "maindefs.h"
#ifndef __XC8
#include <i2c.h>
#else
#include <plib/i2c.h>
#endif
#include "my_i2c.h"

static i2c_comm *ic_ptr;

// Configure for I2C Master mode -- the variable "slave_addr" should be stored in
//   i2c_comm (as pointed to by ic_ptr) for later use.

void i2c_configure_master(unsigned char slave_addr)
{
    
    SSP1CON1bits.SSPM = 0x8;    //Put in master mode

    SSP1STATbits.SMP = 1;       //Put slew rate in standard for normal speed I2C clock
    SSP1STATbits.CKE = 0;       //Disable SNBus input

    SSP1ADD = 0x77;             //Set speed to 100KHz @ 48MHz FOSC

    TRISBbits.TRISB4 = 1;       //Set SCL as output
    TRISBbits.TRISB5 = 1;       //Set SDA as output

    ic_ptr->slave_addr = slave_addr;
    ic_ptr->outbuflen = I2C_DATA_SIZE;
    ic_ptr->status = I2C_IDLE;

    SSP1CON1bits.SSPEN = 1;     //Activate I2C
    
}

// Sending in I2C Master mode [slave write]
// 		returns -1 if the i2c bus is busy
// 		return 0 otherwise
// Will start the sending of an i2c message -- interrupt handler will take care of
//   completing the message send.  When the i2c message is sent (or the send has failed)
//   the interrupt handler will send an internal_message of type MSGT_MASTER_SEND_COMPLETE if
//   the send was successful and an internal_message of type MSGT_MASTER_SEND_FAILED if the
//   send failed (e.g., if the slave did not acknowledge).  Both of these internal_messages
//   will have a length of 0.
// The subroutine must copy the msg to be sent from the "msg" parameter below into
//   the structure to which ic_ptr points [there is already a suitable buffer there].

unsigned char i2c_master_send(unsigned char length, unsigned char *msg) {
    
    if( FromMainHigh_sendmsg(length,MSGT_I2C_DATA,(void *)msg) == MSGQUEUE_FULL )
    {
        return MSGQUEUE_FULL;
    }
    else
    {
        
    }

    PIR1bits.SSP1IF = 1;   //Enable Start Condition; Start the I2C Message
    
    
    return(0);
}

// Receiving in I2C Master mode [slave read]
// 		returns -1 if the i2c bus is busy
// 		return 0 otherwise
// Will start the receiving of an i2c message -- interrupt handler will take care of
//   completing the i2c message receive.  When the receive is complete (or has failed)
//   the interrupt handler will send an internal_message of type MSGT_MASTER_RECV_COMPLETE if
//   the receive was successful and an internal_message of type MSGT_MASTER_RECV_FAILED if the
//   receive failed (e.g., if the slave did not acknowledge).  In the failure case
//   the internal_message will be of length 0.  In the successful case, the
//   internal_message will contain the message that was received [where the length
//   is determined by the parameter passed to i2c_master_recv()].
// The interrupt handler will be responsible for copying the message received into

unsigned char i2c_master_recv(unsigned char length) {
    // Your code goes here
    return(0);
}

void start_i2c_slave_reply(unsigned char length, unsigned char *msg) {

    for (ic_ptr->outbuflen = 0; ic_ptr->outbuflen < length; ic_ptr->outbuflen++) {
        ic_ptr->outbuffer[ic_ptr->outbuflen] = msg[ic_ptr->outbuflen];
    }
    ic_ptr->outbuflen = length;
    ic_ptr->outbufind = 1; // point to the second byte to be sent

    // put the first byte into the I2C peripheral
    SSPBUF = ic_ptr->outbuffer[0];
    // we must be ready to go at this point, because we'll be releasing the I2C
    // peripheral which will soon trigger an interrupt
    SSPCON1bits.CKP = 1; 

}

// an internal subroutine used in the slave version of the i2c_int_handler

void handle_start(unsigned char data_read) {
    ic_ptr->event_count = 1;
    ic_ptr->buflen = 0;
    // check to see if we also got the address
    if (data_read) {
        if (SSPSTATbits.D_A == 1) {
            // this is bad because we got data and
            // we wanted an address
            ic_ptr->status = I2C_IDLE;
            ic_ptr->error_count++;
            ic_ptr->error_code = I2C_ERR_NOADDR;
        } else {
            if (SSPSTATbits.R_W == 1) {
                ic_ptr->status = I2C_SLAVE_SEND;
            } else {
                ic_ptr->status = I2C_RCV_DATA;
            }
        }
    } else {
        ic_ptr->status = I2C_STARTED;
    }
}

// this is the interrupt handler for i2c -- it is currently built for slave mode
// -- to add master mode, you should determine (at the top of the interrupt handler)
//    which mode you are in and call the appropriate subroutine.  The existing code
//    below should be moved into its own "i2c_slave_handler()" routine and the new
//    master code should be in a subroutine called "i2c_master_handler()"

void i2c_int_handler() {


    unsigned char i2c_data;
    //unsigned char length;
    //unsigned char data_read = 0;
    //unsigned char data_written = 0;
    //unsigned char msg_ready = 0;
    unsigned char msg_to_send = MSGT_I2C_DATA;
    //unsigned char overrun_error = 0;
    //unsigned char error_buf[3];
    unsigned char read_bit = 1;        //I2C read/~write bit

    

    switch(ic_ptr->status)
    {
        case I2C_IDLE:
        {
            signed char len = FromMainHigh_recvmsg(I2C_DATA_SIZE, &msg_to_send, (void*)ic_ptr->outbuffer);
            if( len == MSGQUEUE_EMPTY )
            {
                //ic_ptr->error_code=I2C_ERR_NODATA;
                //ToMainHigh_sendmsg(0,MSGT_I2C_MASTER_SEND_FAILED,(void *)&ic_ptr->error_code);
                //ic_ptr->status = I2C_IDLE;
                
                
            }
            else if( len == MSGBUFFER_TOOSMALL )
            {
                
            }
            else
            {
                ic_ptr->outbufind = 0;
                ic_ptr->bufind = 0;
                ic_ptr->outbuflen = len;                
                SSP1CON2bits.SEN = 1;       //Enable Start Condition; Start the I2C Message
                ic_ptr->status = I2C_STARTED;
                
            }
            break;
            
        }
        case I2C_STARTED:
        {
            if( SSP1STATbits.S == 0 )       //Check for started bit off !BAD!
            {
                ic_ptr->status = I2C_IDLE;
                // ADD to main message here
                break;
            }
            else if( SSP1STATbits.BF == 1 )  //Check for SSP1BUF full !BAD!
            {
                ic_ptr->status = I2C_IDLE;
                // ADD to main message here
                break;
            }

            SSP1BUF = ((ic_ptr->slave_addr) << 1);    //Write address


            ic_ptr->status = I2C_MASTER_DATA_SEND;
            ic_ptr->outbufind = 0;
            
            break;
        }
        case I2C_MASTER_DATA_SEND:
        {
            if( SSP1STATbits.BF == 1 )   //Check for SSP1BUF full - wait
            {
                break;
            }

            SSP1BUF = ic_ptr->outbuffer[ic_ptr->outbufind];
            ic_ptr->outbufind++;
            
            if( ic_ptr->outbufind >= I2C_DATA_SIZE )
            {
                unsigned char command_byte = ic_ptr->outbuffer[0];

                switch(command_byte)  //Determine read_bit from command
                {
                    case MSGID_SENSOR_STATUS:
                    {
                        read_bit = 1;
                        break;
                    }
                    default:
                    {
                        read_bit = 0;
                        break;
                    }
                }

                if( read_bit ) // Check if doing a write
                {
                    ic_ptr->status = I2C_MASTER_RESTART;
                    ic_ptr->bufind = 0;
                }
                else
                {
                    ic_ptr->status = I2C_MASTER_DATA_STOP;
                }
            }
            break;
        }
        case I2C_MASTER_RESTART:
        {
            
            SSP1CON2bits.RSEN = 1;

            ic_ptr->status = I2C_MASTER_ADDRESS_RESEND;
            break;
        }
        case I2C_MASTER_ADDRESS_RESEND:
        {
            if( SSP1STATbits.S == 0 )        //Check for started bit off !BAD!
            {
                ic_ptr->status = I2C_IDLE;
                // ADD to main message here
                break;
            }
            else if( SSP1STATbits.BF == 1 )   //Check for SSP1BUF full !BAD!
            {
                ic_ptr->status = I2C_IDLE;
                // ADD to main message here
                break;
            }

            SSP1BUF = ((ic_ptr->slave_addr) << 1)|1;    //Write address

            ic_ptr->status = I2C_MASTER_RECEIVE;
            break;
        }
        case I2C_MASTER_RECEIVE:
        {
            if( SSP1CON2bits.ACKSTAT )  //Check for ACK bit
            {
                SSP1CON2bits.PEN = 1;
                ic_ptr->outbufind = 0;
                ic_ptr->status = I2C_IDLE;
                break;
            }

            SSP1CON2bits.RCEN = 1;

            ic_ptr->outbufind = 0;
            ic_ptr->status = I2C_SLAVE_SEND;
            break;
        }
        case I2C_SLAVE_SEND:
        {
            if( SSP1STATbits.BF == 0 )   //Check for SSP1BUF empty 
            {
                break;
            }

            ic_ptr->buffer[ic_ptr->bufind] = SSP1BUF;
            ic_ptr->bufind++;


            if( ic_ptr->bufind >= I2C_DATA_SIZE )
            {
                signed char err = ToMainHigh_sendmsg(ic_ptr->bufind + 1, MSGT_I2C_DATA , ic_ptr->buffer);
                ic_ptr->status = I2C_MASTER_DATA_STOP;
            }
            else if(SSP1STATbits.P)  //look for stop bit
            {
                signed char err = ToMainHigh_sendmsg(ic_ptr->bufind + 1, MSGT_I2C_DATA , ic_ptr->buffer);
                ic_ptr->status = I2C_MASTER_DATA_STOP;
            }
            else
            {
                SSP1CON2bits.ACKEN = 1;
                ic_ptr->status = I2C_MASTER_RECEIVE;
                break;
            }
        }
        
        case I2C_MASTER_DATA_STOP:
        {
            SSP1CON2bits.PEN = 1;
            ic_ptr->outbufind = 0;
            ic_ptr->outbufind = 0;
            ic_ptr->status = I2C_IDLE;            
        }
    }

}

// set up the data structures for this i2c code
// should be called once before any i2c routines are called

void init_i2c(i2c_comm *ic) {
    ic_ptr = ic;
    ic_ptr->buflen = 0;
    ic_ptr->outbuflen = 0;
    ic_ptr->event_count = 0;
    ic_ptr->status = I2C_IDLE;
    ic_ptr->error_count = 0;
}

// setup the PIC to operate as a slave
// the address must include the R/W bit

void i2c_configure_slave(unsigned char addr) {

    // ensure the two lines are set for input (we are a slave)
#ifdef __USE18F26J50
    //THIS CODE LOOKS WRONG, SHOULDN'T IT BE USING THE TRIS BITS???
    PORTBbits.SCL1 = 1;
    PORTBbits.SDA1 = 1;
#else
#ifdef __USE18F46J50
    TRISBbits.TRISB4 = 1; //RB4 = SCL1
    TRISBbits.TRISB5 = 1; //RB5 = SDA1
#else
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
#endif
#endif

    // set the address
    SSPADD = addr;
    //OpenI2C(SLAVE_7,SLEW_OFF); // replaced w/ code below
    SSPSTAT = 0x0;
    SSPCON1 = 0x0;
    SSPCON2 = 0x0;
    SSPCON1 |= 0x0E; // enable Slave 7-bit w/ start/stop interrupts
    SSPSTAT |= SLEW_OFF;

#ifdef I2C_V3
    I2C1_SCL = 1;
    I2C1_SDA = 1;
#else 
#ifdef I2C_V1
    I2C_SCL = 1;
    I2C_SDA = 1;
#else
#ifdef __USE18F26J50
    PORTBbits.SCL1 = 1;
    PORTBbits.SDA1 = 1;
#else
#ifdef __USE18F46J50
    PORTBbits.SCL1 = 1;
    PORTBbits.SDA1 = 1;
#else
    __dummyXY=35;// Something is messed up with the #ifdefs; this line is designed to invoke a compiler error
#endif
#endif
#endif
#endif
    
    // enable clock-stretching
    SSPCON2bits.SEN = 1;
    SSPCON1 |= SSPENB;
    // end of i2c configure
}