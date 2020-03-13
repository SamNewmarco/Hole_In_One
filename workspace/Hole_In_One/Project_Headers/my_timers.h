/*   My_timers.c contains some time wasting functions.
 * 
 * The looping delays are particularly inaccurate so use with caution



 
 */
#ifndef mytimers_H_
#define mytimers_H_

#include "common.h"

/* Defines and enums */

/********** delay_100us  delay_ms**************/
/*Time wasting delay routines.  Specify a numerical argument, generate a delay roughly eqaul to that value.  For more accurate delays use the crystal clock with the systick interrupt  */

void delay_100us(uint16_t);

void delay_ms(uint16_t);

#endif //mytimers_H_

