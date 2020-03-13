#include "my_sys_ticks.h"

void syst_tick_init(void){//Call at start of program to use sys_ticks
	SYST_RVR=48000;
	SYST_CVR=0;
	SYST_CSR=0x7;
}// End of syst_tick_init

void SysTick_Handler(void){//Interrupt handler
	sys_ticks++;
}// SysTick_Handler