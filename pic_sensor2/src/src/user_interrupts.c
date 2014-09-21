// This is where the "user" interrupts handlers should go
// The *must* be declared in "user_interrupts.h"

#include "maindefs.h"
#ifndef __XC8
#include <timers.h>
#else
#include <plib/timers.h>
#endif
#include "user_interrupts.h"
#include "messages.h"

//const int MSGSIZE = 16;

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
    unsigned int val;
    int length, msgtype;

    // toggle an LED
#ifdef __USE18F2680
    LATBbits.LATB0 = !LATBbits.LATB0;
#endif
    // reset the timer
    WriteTimer0(0);
    // try to receive a message and, if we get one, echo it back
    length = FromMainHigh_recvmsg(sizeof(val), (unsigned char *)&msgtype, (void *) &val);
    if (length == sizeof (val)) {
        ToMainHigh_sendmsg(sizeof (val), MSGT_TIMER0, (void *) &val);
    }
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

void timer1_int_handler() {
    unsigned int result;


    LATAbits.LA0 ^= 0x1;//Test to see if the handler works.

    ADCON0bits.GO_NOT_DONE = 1;////////////////////////////////////Needed to activate the A/D converter interupt

    

    // read the timer and then send an empty message to main()
#ifdef __USE18F2680
    LATBbits.LATB1 = !LATBbits.LATB1;
#endif

    result = ReadTimer1();
    ToMainLow_sendmsg(0, MSGT_TIMER1, (void *) 0);

    // reset the timer
    WriteTimer1(0);
}
/*
void a_to_d_converter_handler() {
    unsigned short mybytes = 0x0000;
    //unsigned char lwbytes;

    mybytes = mybytes | (ADRESH<<8);
    mybytes = mybytes | ADRESL;

    signed char output = ToMainHigh_sendmsg(MSGSIZE, MSGT_TIMER1, &mybytes);

    ADCON0bits.GO_NOT_DONE = 0x0;
    //ADCON0 = ADCON0 ^ 0x02;//xor to set the second to last bit to zero
}
*/