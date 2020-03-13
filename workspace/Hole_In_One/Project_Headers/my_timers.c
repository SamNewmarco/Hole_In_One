/*
 * Timers.c
 *
Very rough delay timers and other timers



 */


#include "my_timers.h"
 



/********** delay_100us  delay_ms**************/
/*Time wasting delay routines.  Specify a numerical argument, generate a delay roughly eqaul to that value.  For more accurate delays use the crystal clock with the systick interrupt  */

void delay_100us(uint16_t x){
	int i=0,j=0;
	
	for(j=0;j<x;j++){
	for(i=0;i<394;i++);
	 }
  }

void delay_ms(uint16_t x){
	int i=0,j=0;
	
	for(j=0;j<x;j++){
	for(i=0;i<10;i++)delay_100us(1);
	 }
  }

