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
    TRISBbits.TRISB4 = 0x0;//RB4 output PIN
    //#endif
    ADCON1bits.VCFG1 = 0x0;//VSS voltage Reference configuration bit (Vref-)
    ADCON1bits.VCFG0 = 0x0; //Vdd Voltage Reference configuration bit (Vref+)
    ADCON1bits.PCFG = 0x8;// Turns on AN0 - AN6

    ADCON0bits.CHS = 0x0;// Sets Select bit AN0

    ADCON2bits.ADFM = 1;//Right Justified
    ADCON2bits.ACQT = 0x7;//20 TAD
    ADCON2bits.ADCS = 0x6;//Fosc/64

    ADCON0bits.ADON = 0x1;// A/D converter module is enabled

    PIR1bits.ADIF = 0x0;
    PIE1bits.ADIE = 0x1;//Enables the A/D interrupt
    INTCONbits.GIE = 0x1;//Enables all unmasked interrupts

    #endif
}
void a_to_d_converter_handler(void) {
    unsigned short mybytes = 0x0000;
    static unsigned char AtoDCntr = 0;
    signed char MsgQState;
#ifdef __USE18F26J50
    #ifdef DEBUG
        LATDbits.LD4 ^= 0x1;
    #endif
#endif
#ifdef __USE18F45J10
    //LATBbits.LB5 ^= 0x1;
#endif
    mybytes = mybytes | (ADRESH<<8);
    mybytes = mybytes | ADRESL;

    //mybytes = rndSense(fake_sensor_cntr++);
    //You set the CHS bit to the next channel at the end of each case.
    switch(AtoDCntr)
    {
        case LB_IR_SENSER:
        {
            MsgQState = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_TO_MAIN_LB_IR, &mybytes);
            ADCON0bits.CHS = 0x1;
            ADCON0bits.GO_NOT_DONE = 0x1;
            break;
        }
        case LF_IR_SENSER:
        {
            MsgQState = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_TO_MAIN_LF_IR, &mybytes);
            ADCON0bits.CHS = 0x2;
            ADCON0bits.GO_NOT_DONE = 0x1;
            break;
        }
        case RB_IR_SENSER:
        {
            MsgQState = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_TO_MAIN_RB_IR, &mybytes);
            ADCON0bits.CHS = 0x3;
            ADCON0bits.GO_NOT_DONE = 0x1;
            break;
        }
        case RF_IR_SENSER:
        {
            MsgQState = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_TO_MAIN_RF_IR, &mybytes);
            ADCON0bits.CHS = 0x0;// Sets Select bit AN2
            ADCON0bits.GO_NOT_DONE = 0x0;
            break;
        }
        /*
        case FL_US_SENSER:
        {
            break;
        }
        case FR_US_SENSER:
        {
            ADCON0bits.CHS = 0x2;// Sets Select bit AN2
            ADCON0bits.GO_NOT_DONE = 0x0;
            break;
        }
        * */
        default:
            //error! Getting here should be impossible.
            break;
    }//end switch.

    //signed char output = ToMainHigh_sendmsg(MSGSIZE, MSGT_AD_CONVERTER_COMPLETE, &mybytes);

    //ADCON0bits.GO_NOT_DONE = 0x0;
    AtoDCntr = (AtoDCntr + 1) % 4;
}

char rndSense(unsigned int index){
    //char myArray[10] = {0x31, 0x32, 0x33, 0x34, 0x35,
    //                  0x36, 0x37, 0x38, 0x39, 0x30};
    char myArray[10] = {0xee, 0xee, 0xee, 0xee, 0xee,
                        0xee, 0xee, 0xee, 0xee, 0xee};
    return myArray[index % 10];
}