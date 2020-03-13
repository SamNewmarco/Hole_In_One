#include "my_PWM.h"

void InitPWM_PORTA2_timer_2_channel_1(void){

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM2_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTA_PCR(2)=PORT_PCR_MUX(3);					//>> Use X n and mux value here.  Its mux 3 usually
		       
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM2_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM2_CONF = 0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM2_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM2_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM COunter
     TPM2_SC |= TPM_SC_CMOD(1);					//>> Use new Y value here (timer 0,1 or 2)
     TPM2_C1SC=0X28;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM2_C1V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
              
}

void PWM_timer_2_channel_1_per(int per){
	 TPM2_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
	
}
void PWM_timer_2_channel_1_pw(int pw){
	 TPM2_C1V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}

void InitPWM_PORTD4_timer_0_channel_4(void){

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTD_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTD_PCR(4)=PORT_PCR_MUX(4);					//>> Use X n and mux value here.  Its mux 3 usually
		       
			
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM0_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM0_CONF = 0x0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM0_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM0_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM COunter
     TPM0_SC |= TPM_SC_CMOD(1);					//>> Use new Y value here (timer 0,1 or 2)
     TPM0_C4SC=0X24;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C4V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	   
}
void PWM_timer_0_channel_4_per(int per){
	TPM0_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
}
void PWM_timer_0_channel_4_pw(int pw){
	TPM0_C4V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)	 	
}

void InitPWM_PORTD0_timer_0_channel_0(void){

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTD_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTD_PCR(0)=PORT_PCR_MUX(4);					//>> Use X n and mux value here.  Its mux 3 usually
		       
			
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM0_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM0_CONF = 0x0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM0_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM0_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM COunter
     TPM0_SC |= TPM_SC_CMOD(1);					//>> Use new Y value here (timer 0,1 or 2)
     TPM0_C0SC=0X28;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C0V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	   
}
void PWM_timer_0_channel_0_per(int per){
	TPM0_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
}
void PWM_timer_0_channel_0_pw(int pw){
	TPM0_C0V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)	 	
}

void InitPWM_PORTE20_timer_1_channel_0(void){

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM1_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTE_PCR(20)=PORT_PCR_MUX(3);					//>> Use X n and mux value here.  Its mux 3 usually
		       
			
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM1_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM1_CONF = 0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM1_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM1_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM COunter
     TPM1_SC |= TPM_SC_CMOD(1);					//>> Use new Y value here (timer 0,1 or 2)
     TPM1_C0SC=0X28;//change this line. to 0x2C?							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM1_C0V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
              
}

void PWM_timer_1_channel_0_per(int per){
	 TPM1_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
	
}
void PWM_timer_1_channel_0_pw(int pw){
	 TPM1_C0V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}