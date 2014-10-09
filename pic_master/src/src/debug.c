#include "debug.h"

void debug_configure()
{
    TRISD = 0x00;
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;

}

void blip()
{
    LATDbits.LD2 = 0;
    LATDbits.LD2 = 1;
    LATDbits.LD2 = 0;
}

void blip1()
{
    LATDbits.LD4 = 0;
    LATDbits.LD4 = 1;
    LATDbits.LD4 = 0;
}

void blip2()
{
    LATDbits.LD5 = 0;
    LATDbits.LD5 = 1;
    LATDbits.LD5 = 0;
}

void blip3()
{
    LATDbits.LD6 = 0;
    LATDbits.LD6 = 1;
    LATDbits.LD6 = 0;
}

void blip4()
{
    LATDbits.LD7 = 0;
    LATDbits.LD7 = 1;
    LATDbits.LD7 = 0;
}

void blink0()
{
    LATBbits.LB0 ^= 1;
}

void blink1()
{
    LATBbits.LB1 ^= 1;
}