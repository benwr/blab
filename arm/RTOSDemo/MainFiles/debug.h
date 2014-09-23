#ifndef DEBUG_H
#define DEBUG_H

#ifdef BLAB_DEBUG
#define DEBUG_OUT(val) {\
        GPIO_SetValue(0, val << 15);\
        GPIO_ClearValue(0, (val << 15) ^ 0x78000);\
}
#else
#define DEBUG_OUT(val)
#endif

#endif
