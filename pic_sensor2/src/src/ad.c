#include "ad.h"
#include "user_interrupts.h"
#include "maindefs.h"
#include "messages.h"

const int MSGSIZE = 4;
unsigned int fake_sensor_cntr;



void init_registers(void)
{
    #ifdef __USE18F26J50
    //Init bits
    TRISAbits.TRISA0 = 0x0;//Output on PIN RA0
    TRISDbits.TRISD4 = 0x0;//Output on PIN RD4
    TRISDbits.TRISD5 = 0x0;//Output on PIN RD5
    TRISDbits.TRISD6 = 0x0;//Output on PIN RD6


    //Configuring the A/D converter
    ADCON0bits.VCFG1 = 0x0;//AVss
    ADCON0bits.VCFG0 = 0x0;//AVdd
    ADCON0bits.CHS = 0x2;//Channel 2 (AN2)
    ADCON0bits.GO_NOT_DONE = 0x0;//Idle

    ADCON1bits.ADFM = 0x1;//Right justified
    ADCON1bits.ADCAL = 0x0;//Normal A/D Converter operation
    ADCON1bits.ACQT = 0x7;//20 TAD
    ADCON1bits.ADCS = 0x6;//Fosc/64

    ADCON0bits.ADON = 0x1;//Activate

    PIR1bits.ADIF = 0x0;
    PIE1bits.ADIE = 0x1;//Enables the A/D interrupt
    INTCONbits.GIE = 0x1;//Enables all unmasked interrupts
    #endif
    #ifdef __USE18F45J10
    //#ifdef DEBUG
    TRISBbits.TRISB7 = 0x0;//RB7 output PIN
    TRISBbits.TRISB6 = 0x0;//RB6 output PIN
    TRISBbits.TRISB5 = 0x0;//RB5 output PIN
    //#endif
    ADCON1bits.VCFG1 = 0x0;//VSS voltage Reference configuration bit (Vref-)
    ADCON1bits.VCFG0 = 0x0; //Vdd Voltage Reference configuration bit (Vref+)
    ADCON1bits.PCFG = 0x6;// Turns on AN0 - AN8

    ADCON0bits.CHS = 0x2;// Sets Select bit AN2

    ADCON2bits.ACQT = 0x2;//4 TAD
    ADCON2bits.ADCS = 0x6;//Fosc/64

    ADCON0bits.ADON = 0x1;// A/D converter module is enabled

    PIR1bits.ADIF = 0x0;
    PIE1bits.ADIE = 0x1;//Enables the A/D interrupt
    INTCONbits.GIE = 0x1;//Enables all unmasked interrupts
    #endif
}

void a_to_d_converter_handler(void) {
    unsigned short mybytes = 0x0000;

#ifdef __USE18F26J50
    #ifdef DEBUG
        LATDbits.LD4 ^= 0x1;
    #endif
#endif
#ifdef __USE18F45J10
     //LATBbits.LB5 ^= 0x1;
#endif
    //mybytes = mybytes | (ADRESH<<8);
    //mybytes = mybytes | ADRESL;

    mybytes = rndSense(fake_sensor_cntr++);


    signed char output = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_CONVERTER_COMPLETE, &mybytes);

    ADCON0bits.GO_NOT_DONE = 0x0;
    //ADCON0 = ADCON0 ^ 0x02;//xor to set the second to last bit to zero
}

char rndSense(unsigned int index){
    char myArray[10] = {0x31, 0x32, 0x33, 0x34, 0x35,
                        0x36, 0x37, 0x38, 0x39, 0x30};
    return myArray[index % 10];
}