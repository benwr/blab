#ifndef __my_i2c_h
#define __my_i2c_h

#include "messages.h"
#include "debug.h"

#define MAXI2CBUF MSGLEN
typedef struct __i2c_comm {
    unsigned char buffer[MAXI2CBUF];
    unsigned char buflen;
    unsigned char bufind;
    unsigned char outbuffer[MAXI2CBUF];
    unsigned char outbufind;
    unsigned char outbuflen;    
    unsigned char event_count;
    unsigned char status;
    unsigned char error_code;
    unsigned char error_count;
    unsigned char initiated;
    unsigned char slave_addr;
} i2c_comm;

#define I2C_DATA_SIZE MSGLEN

#define I2C_IDLE 0x5
#define I2C_STARTED 0x6
#define	I2C_RCV_DATA 0x7
#define I2C_SLAVE_SEND 0x8
#define I2C_MASTER_ADDRESS_SEND 0x9
#define I2C_MASTER_DATA_SEND 0xa
#define I2C_MASTER_DATA_STOP 0xb
#define I2C_MASTER_RESTART 0xc
#define I2C_MASTER_ADDRESS_RESEND 0xd
#define I2C_MASTER_RECEIVE 0xe


#define I2C_ERR_THRESHOLD 1
#define I2C_ERR_OVERRUN 0x4
#define I2C_ERR_NOADDR 0x5
#define I2C_ERR_NODATA 0x6
#define I2C_ERR_MSGTOOLONG 0x7
#define I2C_ERR_MSG_TRUNC 0x8
#define I2C_ERR_NO_ACK 0x9          //No Ack received from slave


void init_i2c(i2c_comm *ic);
void i2c_int_handler(void);
void start_i2c_slave_reply(unsigned char,unsigned char *);
void i2c_configure_slave(unsigned char,unsigned char * ptr_thingy);
void i2c_configure_master(unsigned char);
unsigned char i2c_master_send(unsigned char,unsigned char *);
unsigned char i2c_master_recv(unsigned char);

void retrieve_sensor_values( unsigned char * sensor_bank_1 , unsigned char * sensor_bank_2 , unsigned char * sensor_bank_3 );

#endif