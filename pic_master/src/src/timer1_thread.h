#include "my_i2c.h"
#include "my_uart.h"

typedef struct __timer1_thread_struct {
    // "persistent" data for this "lthread" would go here
    unsigned char move_msg[UART_DATA_LENGTH];
    unsigned char new_move_msg;
    unsigned int msgcount;
} timer1_thread_struct;

void init_timer1_lthread(timer1_thread_struct *);
int timer1_lthread(timer1_thread_struct *,int,int,unsigned char*);
