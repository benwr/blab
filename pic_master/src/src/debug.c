#include "debug.h"

void blip()
{
    LATDbits.LD2 = 0;
    LATDbits.LD2 = 1;
    LATDbits.LD2 = 0;
}