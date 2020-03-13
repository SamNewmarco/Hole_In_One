#ifndef MY_SYS_TICKS_H_
#define MY_SYS_TICKS_H_

#include "derivative.h" /* include peripheral declarations */

void syst_tick_init(void);
void SysTick_Handler(void);

unsigned long sys_ticks;

#endif //MY_SYS_TICKS_H_
