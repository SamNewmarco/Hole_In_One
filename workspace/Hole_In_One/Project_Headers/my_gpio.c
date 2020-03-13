/*   My_gpio.c contains these functions that handle input and output features on our proto board

init_gpio_x(pin, io)  // initialize parallel i/o pin as either input (0) our output (1)  x=a,b,c,d or e
gpi_x(pin)            // returns the value from the input in   x=a,b,c,d or e
gpo_x(pin, value)     // sends the value (1 or 0) to the output pin   x=a,b,c,d or e

 
 */



/* ************************ init_gpio_n(pin,dd)************************
 
 Use these init functions to turn on an io for basic parallel input or output.
 
 int pin; is the pin number fomr 0 to 31
 in dd;   is the data direction  1 for output, 0 for input.
 
ex.  to use the pin A13 for input and D3 for output

init_gpio_a(13,0);
init_gpio_d(3,1);
  
 * 
 * */

#include "my_gpio.h"



void init_gpio_a(int pin,int dd)
{
	
	
// Turn on clock for port A
    
     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;
     
// Convert the specific pins to the GPIO module.  See table 10.3.1 in Users Guide 
     

     PORTA_PCR(pin)=PORT_PCR_MUX(1);
     PORTA_PCR(pin)|=PORT_PCR_PE_MASK;
          
     if(dd==1){
             GPIOA_PDDR|=1<<pin;
               }
     else GPIOA_PDDR&=~(1<<pin);
    }

void init_gpio_b(int pin,int dd)
{
	
// Turn on clock for port B
    
     SIM_SCGC5|=SIM_SCGC5_PORTB_MASK;
     
// Convert the specific pins to the GPIO module. 
     

     PORTB_PCR(pin)=PORT_PCR_MUX(1);
     PORTB_PCR(pin)|=PORT_PCR_PE_MASK;
     
     if(dd==1){
             GPIOB_PDDR|=1<<pin;
               }
     else GPIOB_PDDR&=~(1<<pin);
    }
void init_gpio_c(int pin,int dd)
{
	
// Turn on clock for port C
    
     SIM_SCGC5|=SIM_SCGC5_PORTC_MASK;
     
// Convert the specific pins to the GPIO module. 

     PORTC_PCR(pin)=PORT_PCR_MUX(1);
     PORTC_PCR(pin)|=PORT_PCR_PE_MASK;
     
     if(dd==1){
             GPIOC_PDDR|=1<<pin;
               }
     else GPIOC_PDDR&=~(1<<pin);
    }
void init_gpio_d(int pin,int dd)
{
	
// Turn on clock for port D
    
     SIM_SCGC5|=SIM_SCGC5_PORTD_MASK;
     
// Convert the specific pins to the GPIO module. 
   
     PORTD_PCR(pin)=PORT_PCR_MUX(1);
     PORTD_PCR(pin)|=PORT_PCR_PE_MASK;
     
     if(dd==1){
             GPIOD_PDDR|=1<<pin;
               }
     else GPIOD_PDDR&=~(1<<pin);
    }
void init_gpio_e(int pin,int dd)
{
	
// Turn on clock for port E
    
     SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;
     
// Convert the specific pins to the GPIO module. 
     
     PORTE_PCR(pin)=PORT_PCR_MUX(1);
     PORTE_PCR(pin)|=PORT_PCR_PE_MASK;
     
     if(dd==1){
             GPIOE_PDDR|=1<<pin;
               }
     else GPIOE_PDDR&=~(1<<pin);
    }

/*************  gpi_x and gpo_x    ****************/
/*
gpo_x:  Use these routines to set, or clear a single parallel output

ex, to set the 13th bit on port A and to clear it later
gpo_a(13,1);  //set
.
.
gp_a(13,0);    //clear

gpi_x:  Use these routines to read the value of a specfic bit on a pararllel port

ex.  to read D3 and make a decision based on its value

if(gpi_d(3))....

*/

int gpi_a(int pin)
{
    if(GPIOA_PDIR&(1<<pin))return 1;
    else return 0;
    }
void gpo_a(int pin, int val)
   { 
	if(val==1)GPIOA_PDOR|=1<<pin;
	else if (val==0)GPIOA_PDOR&=~(1<<pin);
	    }
int gpi_b(int pin)
{
    if(GPIOB_PDIR&(1<<pin))return 1;
    else return 0;
    }
void gpo_b(int pin, int val)
   { 
	if(val==1)GPIOB_PDOR|=1<<pin;
	else if (val==0)GPIOB_PDOR&=~(1<<pin);
	    }
int gpi_c(int pin)
{
    if(GPIOC_PDIR&(1<<pin))return 1;
    else return 0;
    }
void gpo_c(int pin, int val)
   { 
	if(val==1)GPIOC_PDOR|=1<<pin;
	else if (val==0)GPIOC_PDOR&=~(1<<pin);
	    }

int gpi_d(int pin)
{
    if(GPIOD_PDIR&(1<<pin))return 1;
    else return 0;
    }
void gpo_d(int pin, int val)
   { 
	if(val==1)GPIOD_PDOR|=1<<pin;
	else if (val==0)GPIOD_PDOR&=~(1<<pin);
	    }

int gpi_e(int pin)
{
    if(GPIOE_PDIR&(1<<pin))return 1;
    else return 0;
    }
void gpo_e(int pin, int val)
   { 
	if(val==1)GPIOE_PDOR|=1<<pin;
	else if (val==0)GPIOE_PDOR&=~(1<<pin);
	    }

