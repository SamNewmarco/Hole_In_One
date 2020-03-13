/*
 * Keypad.h
 *
 *  Created on: Jul 17, 2017
 *      Author: Brendan
 *       *******************************************************************            
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
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "common.h"
#include "my_gpio.h"
#include "my_timers.h"


#define bitsInKey   0b0000000000111100
#define bitsInKey1  0b0000000000000100
#define bitsInKey2  0b0000000000001000
#define bitsInKey3  0b0000000000010000
#define bitsInKey4  0b0000000000100000
#define bitsOutKey  0b0000111100000000
#define bitsOutKey1 0b0000000100000000
#define bitsOutKey2 0b0000001000000000
#define bitsOutKey3 0b0000010000000000
#define bitsOutKey4 0b0000100000000000


void initKey(void);  // initiaze the keypad
uint8_t anyKey(void);   //return a 1 if anyeky is pushed on the keypad
uint8_t getKey(void);    //return the ascii character of the keypad button pushed.

#endif /* KEYPAD_H_ */
