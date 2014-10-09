#include "maindefs.h"
#include <stdio.h>
#ifndef __XC8
#include <usart.h>
#include <i2c.h>
#include <timers.h>
#else
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#endif
#include "interrupts.h"
#include "messages.h"
//#include "my_uart.h"
#include "my_i2c.h"
#include "uart_thread.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "ad.h"



//Setup configuration registers
#ifdef __USE18F45J10
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow disabled)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#else
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = HSPLL     // Oscillator Selection bits (HS oscillator, PLL enabled and under software control)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#else

#ifdef __USE18F2680
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif

#else

#ifdef __USE18F26J50

// PIC18F26J50 Configuration Bit Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 3       // PLL Prescaler Selection bits (Divide by 3 (12 MHz oscillator input))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset  (Disabled)
#pragma config XINST = ON       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL      // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = T1OSCREF// DSWDT Clock Select (DSWDT uses T1OSC/T1CKI)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)

#else

#ifdef __USE18F46J50

// PIC18F46J50 Configuration Bit Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 3       // PLL Prescaler Selection bits (Divide by 3 (12 MHz oscillator input))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset (Disabled)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set (Enabled)
#else
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)
#endif

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL      // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = T1OSCREF// DSWDT Clock Select (DSWDT uses T1OSC/T1CKI)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)

#else

Something is messed up.
The PIC selected is not supported or the preprocessor directives are wrong.

#endif
#endif
#endif
#endif



void main(void) {
    char c;
    signed char length;
    unsigned char msgtype;
    unsigned char last_reg_recvd;
    //uart_comm uc;
    i2c_comm ic;
    unsigned char msgbuffer[MSGLEN + 1];
    unsigned char i;
    uart_thread_struct uthread_data; // info for uart_lthread
    timer1_thread_struct t1thread_data; // info for timer1_lthread
    timer0_thread_struct t0thread_data; // info for timer0_lthread
    unsigned char i2c_need_data = 1;

#ifdef __USE18F2680
    OSCCON = 0xFC; // see datasheet
    // We have enough room below the Max Freq to enable the PLL for this chip
    OSCTUNEbits.PLLEN = 1; // 4x the clock speed in the previous line
#else
#ifdef __USE18F45J10
    OSCCON = 0x82; // see datasheeet
    OSCTUNEbits.PLLEN = 0; // Makes the clock exceed the PIC's rated speed if the PLL is on
#else
#ifdef __USE18F26J50
    OSCCON = 0xE0; // see datasheeet
    OSCTUNEbits.PLLEN = 1;
#else
#ifdef __USE18F46J50
    OSCCON = 0xE0; //see datasheet
    OSCTUNEbits.PLLEN = 1;
#else
    Something is messed up.
    The PIC selected is not supported or the preprocessor directives are wrong.
#endif
#endif
#endif
#endif

    

    // initialize the i2c code
    init_i2c(&ic);

    // init the timer1 lthread
    init_timer1_lthread(&t1thread_data);

    // initialize message queues before enabling any interrupts
    init_queues();

#ifndef __USE18F26J50
    // set direction for PORTB to output
    TRISB = 0x0;
    LATB = 0x0;
#endif

    // how to set up PORTA for input (for the V4 board with the PIC2680)
    /*
            PORTA = 0x0;	// clear the port
            LATA = 0x0;		// clear the output latch
            ADCON1 = 0x0F;	// turn off the A2D function on these pins
            // Only for 40-pin version of this chip CMCON = 0x07;	// turn the comparator off
            TRISA = 0x0F;	// set RA3-RA0 to inputs
     */

    // initialize Timers
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
    
#ifdef __USE18F26J50
    // MTJ added second argument for OpenTimer1()
    OpenTimer1(TIMER_INT_ON & T1_SOURCE_FOSC_4 & T1_PS_1_8 & T1_16BIT_RW & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF,0x0);
#else
#ifdef __USE18F46J50
    OpenTimer1(TIMER_INT_ON & T1_SOURCE_FOSC_4 & T1_PS_1_8 & T1_16BIT_RW & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF,50000);
#else
    OpenTimer1(TIMER_INT_ON & T1_PS_1_2 & T1_16BIT_RW & T1_SOURCE_INT & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
#endif
#endif

    // Decide on the priority of the enabled peripheral interrupts
    // 0 is low, 1 is high
    // Timer1 interrupt
    IPR1bits.TMR1IP = 0;
    // USART RX interrupt
    IPR1bits.RCIP = 0;
    // I2C interrupt
    IPR1bits.SSPIP = 1;

    // configure the hardware i2c device as a slave (0x9E -> 0x4F) or (0x9A -> 0x4D)
#if 1
    // Note that the temperature sensor Address bits (A0, A1, A2) are also the
    // least significant bits of LATB -- take care when changing them
    // They *are* changed in the timer interrupt handlers if those timers are
    //   enabled.  They are just there to make the lights blink and can be
    //   disabled.
    i2c_configure_slave(0x9E,&i2c_need_data);
#else
    // If I want to test the temperature sensor from the ARM, I just make
    // sure this PIC does not have the same address and configure the
    // temperature sensor address bits and then just stay in an infinite loop
    i2c_configure_slave(0x9A);
#ifdef __USE18F2680
    LATBbits.LATB1 = 1;
    LATBbits.LATB0 = 1;
    LATBbits.LATB2 = 1;
#endif
    for (;;);
#endif

    // must specifically enable the I2C interrupts
    PIE1bits.SSPIE = 1;

    // configure the hardware USART device
#ifdef __USE18F26J50
    //Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 0x19);
#else
#ifdef __USE18F46J50
    //Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 0x19);
    
    
#else
    //OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 0x19);
#endif
#endif

    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    enable_interrupts();
    //uart_configure();

    /* Junk to force an I2C interrupt in the simulator (if you wanted to)
    PIR1bits.SSPIF = 1;
    _asm
    goto 0x08
    _endasm;
     */

    
    init_registers();//Luke's code
    //[0] is the cmd/id, [1] is a recerved byte, [2-5] are sensor data values
    unsigned char updateMask [UPDATE_MASK_SIZE] = {0x00, 0x00};
    unsigned char snsmsgbuf [SENS_CMD_SIZE] = {SENSOR_RESPONSE, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0};
    

    // printf() is available, but is not advisable.  It goes to the UART pin
    // on the PIC and then you must hook something up to that to view it.
    // It is also slow and is blocking, so it will perturb your code's operation
    // Here is how it looks: printf("Hello\r\n");


    // loop forever
    // This loop is responsible for "handing off" messages to the subroutines
    // that should get them.  Although the subroutines are not threads, but
    // they can be equated with the tasks in your task diagram if you
    // structure them properly.
    //unsigned short current_distance = 0x0000;
    //To Indicate the start of the while loop.
    LATBbits.LB7 = 0x1;
    LATBbits.LB7 = 0x0;
    unsigned int cntr = 0;
    while (1) {
        
        // Call a routine that blocks until either on the incoming
        // messages queues has a message (this may put the processor into
        // an idle mode)
        block_on_To_msgqueues();
        
        if( i2c_need_data )
        {
            signed char MsgQ_BStatus = FromMainHigh_sendmsg(SENS_CMD_SIZE, MSGT_I2C_DATA, snsmsgbuf);
            
            if(MsgQ_BStatus != MSGSEND_OKAY)
            {
                //Error. Message note ok.
            }
            
            i2c_need_data = 0;
        }

        // At this point, one or both of the queues has a message.  It
        // makes sense to check the high-priority messages first -- in fact,
        // you may only want to check the low-priority messages when there
        // is not a high priority message.  That is a design decision and
        // I haven't done it here.
        length = ToMainHigh_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // This case be handled by your code.
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER0:
                {
                    timer0_lthread(&t0thread_data, msgtype, length, msgbuffer);
                    break;
                };
                case MSGT_I2C_DATA:
                case MSGT_I2C_DBG:
                {
                    // Here is where you could handle debugging, if you wanted
                    // keep track of the first byte received for later use (if desired)
                    last_reg_recvd = msgbuffer[0];
                    break;
                };
                case MSGT_I2C_RQST:
                {
                    
                    /*
                    // Generally, this is *NOT* how I recommend you handle an I2C slave request
                    // I recommend that you handle it completely inside the i2c interrupt handler
                    // by reading the data from a queue (i.e., you would not send a message, as is done
                    // now, from the i2c interrupt handler to main to ask for data).
                    //
                    // The last byte received is the "register" that is trying to be read
                    // The response is dependent on the register.
                    switch (last_reg_recvd) {
                        case 0xaa:
                        {
                            length = 4;
                            msgbuffer[0] = 0x55;
                            msgbuffer[1] = 0xAA;


                            break;
                        }
                        case 0xa8:
                        {
                            length = 1;
                            msgbuffer[0] = 0x3A;
                            break;
                        }
                        case 0xa9:
                        {
                            length = 1;
                            msgbuffer[0] = 0xA3;
                            break;
                        }
                    };
                    start_i2c_slave_reply(length, msgbuffer);
                    */
                    break;
                };
                case MSGT_AD_CONVERTER_COMPLETE:
                {
                #ifdef __USE18F26J50
                #ifdef DEBUG
                    LATDbits.LD6 ^= 0x1;
                #endif
                #endif

                #ifdef __USE18F45J10
                //#ifdef DEBUG
                    //LATBbits.LB6 ^= 0x1;
                //#endif
                #endif

                    unsigned short sensor_value = msgbuffer[1];
                    sensor_value = ( sensor_value << 8 ) | msgbuffer[0];

                    unsigned char distance = (18924/( sensor_value - 17 ));

                    unsigned char FLUltra = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | FL_ULTRA_UPDATE;
                    unsigned char FRUltra = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | FR_ULTRA_UPDATE;
                    unsigned char LSFIR = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | LSF_IR_UPDATE;
                    unsigned char LSRIR = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | LSR_IR_UPDATE;
                    unsigned char RSFIR = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | RSF_IR_UPDATE;
                    unsigned char RSRIR = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | RSR_IR_UPDATE;
                    unsigned char FLine = rndSense(cntr++);
                    updateMask[0] = updateMask[0] | F_LINE_UPDATE;
                    unsigned char RLine = rndSense(cntr++);
                    updateMask[1] = updateMask[1] | R_Line_UPDATE;
                    unsigned char RFID = rndSense(cntr++);
                    updateMask[1] = updateMask[1] | RFID_UPDATE;
                    ///myData.distance1 = (18924/( sensor_value - 17 ));
                    
                    
                    //snsmsgbuf[0] = SENSER_CMD;
                    snsmsgbuf[1] = updateMask[0];
                    snsmsgbuf[2] = updateMask[1];
                    snsmsgbuf[3] = FLUltra;
                    snsmsgbuf[4] = FRUltra;
                    snsmsgbuf[5] = LSFIR;
                    snsmsgbuf[6] = LSRIR;
                    snsmsgbuf[7] = RSFIR;
                    snsmsgbuf[8] = RSRIR;
                    snsmsgbuf[9] = FLine;
                    snsmsgbuf[10] = RLine;
                    snsmsgbuf[11] = RFID;

                    signed char MsgQB_Status = FromMainHigh_sendmsg(SENS_CMD_SIZE, MSGT_I2C_DATA, snsmsgbuf);

                    //Reset the update mask to signafy that the current values are now old.
                    updateMask[0] = 0x00;
                    updateMask[1] = 0x00;
                    /*
                    if(fntmsgbuf[2] == distance)
                      fntmsgbuf[1] = fntmsgbuf[1] & 0x7f;
                    else
                        fntmsgbuf[1] = fntmsgbuf[1] | 0x80;
                    if(fntmsgbuf[3] == distance)
                        fntmsgbuf[2] = fntmsgbuf[2] & 0xbf;
                    else
                        fntmsgbuf[1] = fntmsgbuf[1] | 0x40;
                    fntmsgbuf[2] = distance;
                    fntmsgbuf[3] = distance;
                    fntmsgbuf[4] = 0xaa;
                    fntmsgbuf[5] = 0xaa;

                    //signed char MsgQ_BStatus = FromMainHigh_sendmsg(6, MSGT_I2C_DATA, mymsgbuf);

                    sidmsgbuf[0] = SIDE_CMD;
                    sidmsgbuf[1] = 0x00;
                    sidmsgbuf[2] = distance;
                    sidmsgbuf[3] = distance;
                    sidmsgbuf[4] = distance;
                    sidmsgbuf[5] = distance;

                    //MsgQ_BStatus = FromMainHigh_sendmsg(6, MSGT_I2C_DATA, mymsgbuf);

                    vntmsgbuf[0] = VENTRIL_CMD;
                    vntmsgbuf[1] = 0x00;
                    vntmsgbuf[2] = distance;
                    vntmsgbuf[3] = distance;
                    vntmsgbuf[4] = distance;
                    vntmsgbuf[5] = 0xaa;
                    */

                    //MsgQ_BStatus = FromMainHigh_sendmsg(6, MSGT_I2C_DATA, mymsgbuf);
                };
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }

        // Check the low priority queue
        length = ToMainLow_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // Your code should handle this situation
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER1:
                {
                    timer1_lthread(&t1thread_data, msgtype, length, msgbuffer);
                    break;
                };
                case MSGT_OVERRUN:
                case MSGT_UART_DATA:
                {
                    uart_lthread(&uthread_data, msgtype, length, msgbuffer);
                    break;
                };
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }
    }

}