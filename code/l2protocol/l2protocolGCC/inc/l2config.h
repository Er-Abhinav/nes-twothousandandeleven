#ifndef L2CONFIG_H
#define L2CONFIG_H

// l1 settings
#define MODE_8N1 1234 // replace with: #include "../hwusart.h"

#define L2CONFIG_UART_MODE MODE_8N1
#define L2CONFIG_UART_BAUD 9600


// queue settings
#define L2CONFIG_RECEIVEQUEUE_LEN 3
#define L2CONFIG_SENDQUEUE_LEN 3

#endif
