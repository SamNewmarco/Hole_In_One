/*   My_adc.c contains these functions that handle input and output features on our proto board

init_adc0()         //initialize the 0 analog to digital converter
adc0_value(channel)  // returns the 12 bit value for the specified channel

 
 */



#include "my_adc.h"

 /****************  ADC0  initialization  ********************************/
//Initialize PTC1 as ADC15  A to D 
void init_adc0(void){    
 
      // Turn on the clock for ADC0	
        SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;


         ADC0_SC1A=0x0;     //start ot with no channel selected
         ADC0_SC2=0;
         ADC0_CFG1=0x4;   //  12 bit conversion, single ended, full speed bus clock normal operation

}

/*****************ADC0 channel select*****************************/

/* adc0_value returns a 12 bit value proportional to the analog value at the pin selected.
   The partial list is:   USB at top
   
   6x2 pin header
                     -------
    
    ADC0_8	PTB0	---------		PTE20	ADC0_0
    	9	B1		|	|	|		E21		4A
    	12	B2		|	|	|		E22		3
    	13	B3		|	|	|		E23		7A
    	11	C2		|	|	|		E29		4B
    	15	C1		|	|	|		E30		23
    	            ---------
    
                   
   10x2 pin header                
  
   	   --	PTD7	---------		PTE1	ADC0_0
    	7B	D6		|	|	|		E0		--
    	--	NC		|	|	|		AREF	--
    	--	E31		|	|	|		GND		--
    	--	A17		|	|	|		D1		--
    	--	A16		|	|	|		D3		--
    	--	C17		|	|	|		D2		--
    	--	C16		|	|	|		D0		--
    	--	C13		|	|	|		D5		6B     ****  Trimpot connected here!!!
    	--	C12		|	|	|		A13		--
    	            ---------
  
  
  
                     
     For example, if you want to use the PTB2 pin as an atod
     
     init_adc0();
     
     
     x=adc0_value(12,'A');  will return the analog value.
     
 */

int adc0_value(int adc0_se,char aB){
    	

		if((aB=='B')||(aB=='b'))ADC0_CFG2=0x10;
		else ADC0_CFG2=0x0;
    	
    	
    	ADC0_SC1A=adc0_se;

	
	  while(!(ADC_SC1_COCO_MASK&ADC0_SC1A));
	

	  return ADC0_RA;
}
