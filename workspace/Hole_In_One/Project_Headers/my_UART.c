#include "my_UART.h"

void UART0_Init(void)
{
  SIM_SOPT2 = SIM_SOPT2_UART0SRC(1);
  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;  //selects 24MHz baud generator source clock
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
  PORTA_PCR1 = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x2);  //select virtual com option
  PORTA_PCR2 = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x2);
  
  UART0_C2 &= ~ (UART0_C2_TE_MASK| UART0_C2_RE_MASK);//disable Tx, Rx; then change controls
  enable_irq(12); set_irq_priority(12, 3);
  UART0_BDH = 0x00; //Baud Divisor, high and low portions
  UART0_BDL = 0x0D;	//0x1A gives 57600 baud, 0x0D give 115200 baud
  	  	  	  	  //Baud Rate = Source clock/((OverSamp+1)*BaudDivisor)
  UART0_C4 = 0x0F;//oversampling clock ratio
  UART0_C1 = 0x00;
  UART0_C3 = 0x00;
  UART0_MA1 = 0x00;
  UART0_MA2 = 0x00;
  UART0_S1 |= 0x1F;//clears all interrupt flags
  UART0_S2 |= 0xC0;

  UART0_C2 = UART0_C2_RIE_MASK;
  UART0_C2 |= (UART0_C2_TE_MASK| UART0_C2_RE_MASK);
}//End of UART0_Init

void UART0_IRQHandler(void){//Handler for the serial interrupt.
  
  if (UART0_S1&UART_S1_RDRF_MASK){//Look for serial input. 
	char serial_buf = UART0_D;//Load input into buffer.

    if ((UART0_S1&UART_S1_TDRE_MASK)||(UART0_S1&UART_S1_TC_MASK)){
	    UART0_D  = serial_buf;//Send input back.
			
	 }//End send if
  }//End receive if

}//End UART0_IRQHandler

int UART0_Transmitter_Ready(void){//tests to see if ok to send, returns a 1 if ok, 0 if not ok.
 if ((UART0_S1&UART_S1_TDRE_MASK)&&(UART0_S1&UART_S1_TC_MASK))
	 return (1);
 else return (0);
}//End of UART0_Transmitter_Ready

void UART0_Transmit(char tc){ //sends one char to UART0, waits for previous one to finish
	while (UART0_Transmitter_Ready() == 0); //waits till UART done with previous transmission
		UART0_D = tc; //sends character back to the terminal window
    return;
}//End of UART0_Transmit

void UART0_Transmit_String(char *sptr){ //sends complete string to UART0
	while (*sptr){
		UART0_Transmit(*sptr);
		sptr++;
	}//End of while
	return;
}//End of UART0_Transmit_String
