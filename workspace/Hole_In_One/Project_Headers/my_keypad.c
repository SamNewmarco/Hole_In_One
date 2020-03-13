/*
 * Keypad.c
 *
 *  Created on: Jul 17, 2017
 *      Author: Brendan
 */
/*
 *******************************************************************            
 This program assumes this connectivity to ports E and B
 If we input to the row pins, B8:1, then we can 
 read E2:5 as input.  Since the input pins are internally
 tied high, we can look for low values.

  Look at the board from front, orient the pins at the
  bottom, count left to right.
   
  
      b8  b9  b10 b11
    +---+---+---+---+
 e2 | 1 | 2 | 3 | A |
    +---+---+---+---+
 e3 | 4 | 5 | 6 | B |
    +---+---+---+---+
 e4 | 7 | 8 | 9 | C |
    +---+---+---+---+
 e5 | * | 0 | # | D |
    +---+---+---+---+ 

   1  2  3  4  5  6  7  8  
   o  o  o   o     o  o  o  o
   b8 b9 b10 b11   e2 e3 e4 e5 

 * */; 


#include "my_keypad.h"
//**********  Initialize the Keypad    ***********************

void initKey(void){
    init_gpio_e(2,1);   //output from MCU input to the keypad
    init_gpio_e(3,1);
    init_gpio_e(4,1);
    init_gpio_e(5,1);
    
    gpo_e(2,1);
    gpo_e(3,1);
    gpo_e(4,1);
    gpo_e(5,1);
    
    init_gpio_b(8,0);    //input to MCU output from keypad
    init_gpio_b(9,0);
    init_gpio_b(10,0);
    init_gpio_b(11,0);

}

uint8_t anyKey(void){
	uint16_t temp=0;
	GPIOE_PDOR&=~bitsInKey; //  Pull inputs low
	delay_ms(1);
	temp=GPIOB_PDIR;
	temp&=bitsOutKey;  //clear all bits except keypad bits 
	if(temp!=bitsOutKey)return 1;
	else return 0;
}

uint8_t getKey(void){
	
		uint16_t temp=0;
		
		GPIOE_PDOR|=bitsInKey;
		//delay_ms(1);
			
		GPIOE_PDOR&=~bitsInKey1;
		//delay_ms(1);
		    
		temp=GPIOB_PDIR;
		temp&=bitsOutKey;  //clear all other bits except keypad bits
		if(!(temp&bitsOutKey1))return '1';
		else if(!(temp&bitsOutKey2))return '2';
		else if(!(temp&bitsOutKey3))return '3';
		else if(!(temp&bitsOutKey4))return 'A';
//****************************************************			
		GPIOE_PDOR&=~bitsInKey2;
		//delay_ms(1);

		
		temp=GPIOB_PDIR;
		temp&=bitsOutKey;  //clear all other bits except keypad bits
		if(!(temp&bitsOutKey1))return '4';
		else if(!(temp&bitsOutKey2))return '5';
		else if(!(temp&bitsOutKey3))return '6';
		else if(!(temp&bitsOutKey4))return 'B';
		
//****************************************************			
		GPIOE_PDOR&=~bitsInKey3;
		//delay_ms(1);

		
		temp=GPIOB_PDIR;
		temp&=bitsOutKey;  //clear all other bits except keypad bits
		if(!(temp&bitsOutKey1))return '7';
		else if(!(temp&bitsOutKey2))return '8';
		else if(!(temp&bitsOutKey3))return '9';
		else if(!(temp&bitsOutKey4))return 'C';
		
//****************************************************			
		GPIOE_PDOR&=~bitsInKey4;
		delay_ms(1);

		temp=GPIOB_PDIR;
		temp&=bitsOutKey;  //clear all other bits except keypad bits
		if(!(temp&bitsOutKey1))return '*';
		else if(!(temp&bitsOutKey2))return '0';
		else if(!(temp&bitsOutKey3))return '#';
		else if(!(temp&bitsOutKey4))return 'D';
		
//****************************************************			
		
		return 0;
}


