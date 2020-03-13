/*   My_io_apps.c contains these functions that handle input and output features on our proto board

init_gpio_x(pin, io)  // initialize parallel i/o pin as either input (0) our output (1)  x=a,b,c,d or e
gpi_x(pin)            // returns the value from the input in   x=a,b,c,d or e
gpo_x(pin, value)     // sends the value (1 or 0) to the output pin   x=a,b,c,d or e

 
 */
#ifndef mygpio_H_
#define mygpio_H_

#include "common.h"

/* Defines and enums */


void init_gpio_a(int,int);
void init_gpio_b(int,int);
void init_gpio_c(int,int);
void init_gpio_d(int,int);
void init_gpio_e(int,int);
int gpi_a(int);
int gpi_b(int);
int gpi_c(int);
int gpi_d(int);
int gpi_e(int);

void gpo_a(int,int);
void gpo_b(int,int);
void gpo_c(int,int);
void gpo_d(int,int);
void gpo_e(int,int);




#endif //mygpio_H_

