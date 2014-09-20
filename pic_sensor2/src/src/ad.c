#include "ad.h"
#include "user_interrupts.h"
#include "maindefs.h"
#include "messages.h"

const int MSGSIZE = 10;

void init_registers(void)
{
    //Init bits
    TRISAbits.TRISA0 = 0x0;//Output on PIN RA0
    TRISDbits.TRISD4 = 0x0;//Output on PIN RD4
    TRISDbits.TRISD5 = 0x0;//Output on PIN RD5
    TRISDbits.TRISD6 = 0x0;//Output on PIN RD6


    //Configuring the A/D converter
    ADCON0bits.VCFG1 = 0x0;//AVss
    ADCON0bits.VCFG0 = 0x0;//AVdd
    ADCON0bits.CHS = 0x2;//Channel 10 (AN10)
    ADCON0bits.GO_NOT_DONE = 0x0;//Idle

    ADCON1bits.ADFM = 0x1;//Right justified
    ADCON1bits.ADCAL = 0x0;//Normal A/D Converter operation
    ADCON1bits.ACQT = 0x7;//20 TAD
    ADCON1bits.ADCS = 0x6;//Fosc/64

    ADCON0bits.ADON = 0x1;//Activate

    PIR1bits.ADIF = 0x0;
    PIE1bits.ADIE = 0x1;//Enables the A/D interrupt
    INTCONbits.GIE = 0x1;//Enables all unmasked interrupts
}

void a_to_d_converter_handler() {
    unsigned short mybytes = 0x0000;

    LATDbits.LD4 ^= 0x1;
    mybytes = mybytes | (ADRESH<<8);
    mybytes = mybytes | ADRESL;


    LATDbits.LD5 ^= 0x1;
    signed char output = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_CONVERTER_COMPLETE, &mybytes);

    ADCON0bits.GO_NOT_DONE = 0x0;
    //ADCON0 = ADCON0 ^ 0x02;//xor to set the second to last bit to zero
}