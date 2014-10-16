#include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"




void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->msgcount = 0;
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    signed char retval;
    blink0();

    unsigned char sensor_request[14];
    sensor_request[0]=0x02;

    i2c_master_send(14,sensor_request);

    static unsigned char move_counter = 0;

    if( tptr->new_move_msg )
    {
        tptr->new_move_msg = 0;
        move_counter = tptr->move_msg[4];
    }

    if( move_counter != 0 )
    {
        i2c_master_send(14,tptr->move_msg);
        move_counter--;
    }

    
    
    
    tptr->msgcount++;
    // Every tenth message we get from timer1 we
    // send something to the High Priority Interrupt
    if ((tptr->msgcount % 10) == 9) {
        retval = FromMainHigh_sendmsg(sizeof (tptr->msgcount), MSGT_MAIN1, (void *) &(tptr->msgcount));
        if (retval < 0) {
            // We would handle the error here
        }
    }
    
}