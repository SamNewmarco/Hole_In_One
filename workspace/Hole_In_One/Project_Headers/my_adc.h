/*   My_adc.c contains these functions that handle input and output features on our proto board
 
init_adc0()         //initialize the 0 analog to digital converter
adc0_value(channel)  // returns the 12 bit value for the specified channel
 
 */

#ifndef my_adc_H_
#define my_adc_H_

#include "common.h"


void init_adc0(void);

int adc0_value(int,char);

#endif //myadc_H_
