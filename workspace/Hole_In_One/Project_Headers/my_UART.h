#ifndef my_UART_H_
#define my_UART_H_

#include "derivative.h"
#include "arm_cm0.h"

void UART0_Init(void);
void UART0_IRQHandler(void);
int UART0_Transmitter_Ready(void);
void UART0_Transmit(char tc);
void UART0_Transmit_String(char *sptr);


#endif //my_UART_H_
