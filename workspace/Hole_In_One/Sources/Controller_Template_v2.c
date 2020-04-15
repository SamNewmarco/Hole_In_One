/*
 * Controller Template, v.2
 * Luzardo/St.Denis Nov. 2018)
 * 
 * Implementation includes 4 analog to digital inputs (trimpot, PTB0,PTB1,and PTB2)
 * and 2 PWM outputs (PTA2 and PTD4).
 * Use PTD4 (for PWM) and PTC3 (for direction) to control L298 motor controller.
 * Use PTA2 (for PWM) and PTC0 (for direction) to control L298 motor controller.
 * 
 * Also the LEDs, pushbuttons, keypad, and display on the ELT2050 add-on board
 * are activated. 
 * Additionally, multi-digit (integer) input on keypad is implemented. Keys 'C'
 * and 'D' update Kp and Kd.
 * 
 * (FYI: From experimentation, it takes about 800useconds to display each char 
 * on the OLED. A complete line of 18 characters take over 10millisec so be 
 * careful how much you display in each loop. Also, 'clearing the screen' is accomplished 
 * by writing 8x18 spaces (takes a very long time). All other I/O is under a millisecond. 
 */

#include "derivative.h" /* include peripheral declarations */
#include "mcg.h"      /* using XTAL and PLL does not work returns errors*/
#include "my_gpio.h" /* include peripheral declarations */
#include "my_adc.h"
#include "my_ssd1306.h"
#include "my_keypad.h"
#include "my_timer_apps.c"
#include "my_systicks.c"
#include <stdio.h>
#include <stdlib.h>

//****Some useful macros****
#define BUTTON1 (gpi_c(4))
#define BUTTON2 (gpi_c(5))
#define BUTTON3 (gpi_c(6))
#define BUTTON4 (gpi_c(7))
#define TURN_OFF_LED1 gpo_c(12,1)
#define TURN_ON_LED1  gpo_c(12,0)
#define TURN_OFF_LED2 gpo_c(13,1)
#define TURN_ON_LED2  gpo_c(13,0)
#define TURN_OFF_LED3 gpo_c(16,1)
#define TURN_ON_LED3  gpo_c(16,0)
#define TURN_OFF_LED4 gpo_c(17,1)
#define TURN_ON_LED4  gpo_c(17,0)

#define LOOPTIME 10

void Check_Keypad(void);
void Initialize_All(void);
void Set_PWM_D4(int);
void Set_PWM_A2(int);
/**************Global Variables Here************************/
int x,y=0;
int trimpot_result=0;
int adc_ch8_result=0; //signal on PTB0
int adc_ch9_result=0; //signal on PTB1
int adc_ch12_result=0;//signal on PTB2
int PWM_D4_period = 50000;//x 5.3usec = 266millisec
int PWM_D4_pw = 5000;
int PWM_A2_period = 3000; //x 5.33usec =16millisec
int PWM_A2_pw = 150;
//controller variables
int error;
int Kp, Kd;

char keystroke, lastkey;
char keybuffer[10]; //for multi-digit inputs
int kbindex=0;

char Disp_Buffer[30];
int CPU_load; //rough calc of CPU load using sys_ticks and loop interval
int CPU_load_overrun_count=0; //just increment this to flag that there's an issue.
int FakeTaskTime = 5; //just to waste time in the loop to test CPU load calc

int main(void) {
	Initialize_All();
	
   //*****splash screen********************
    clearDisplay();
    writeString ("Controller Test");
    setLocation(0,1); //second line on display
    writeString ("..press PB1   ");
    while (BUTTON1 == 1);
    while (BUTTON1 == 0); //waits for a press and release of PB1
    setLocation(0,1); //second line on display
    writeString ("              ");   
    
	for(;;) {	 //*****main controller loop*********
		sys_ticks = 0;
		trimpot_result=adc0_value(6,'b');	//possible "ref" input	
		adc_ch8_result=adc0_value(8,'a');   //signal on PTB0
		adc_ch9_result=adc0_value(9,'a');   //signal on PTB1
		adc_ch12_result=adc0_value(12,'a');	//reads sensor on PTB2		

		Check_Keypad(); //just to demo it works, displays the ADC results by pressing 'A'&'B'
				
		//******doing something with the PWMs********
//		if (BUTTON1==0) 
//			if (PWM_D4_pw < PWM_D4_period) PWM_D4_pw+=300; //don't press button too long
//		if (BUTTON2==0) 
//			if (PWM_D4_pw > 300) PWM_D4_pw-=300; //don't press button too long
//		if (BUTTON3==0) 
//			if (PWM_A2_pw < PWM_A2_period) PWM_A2_pw+=50; //don't press button too long
//		if (BUTTON4==0) 
//			if (PWM_A2_pw > 50) PWM_A2_pw-=50; //don't press button too long		
//		PWM_timer_2_channel_1_pw(PWM_A2_pw);  //updating the PWM pulse widths
//	    PWM_timer_0_channel_4_pw(PWM_D4_pw);	
	    
		
		//****** doing something to implement position control, proportional only.
		error = trimpot_result - (Kp*adc_ch8_result);
		Set_PWM_D4(error);  // scales the error to the PWM range; sets the direction on PTC3 
	    
			delay_ms(FakeTaskTime);	    
			CPU_load = (sys_ticks*100)/LOOPTIME; //good to check to see if you're not overrunning the interval
			if(CPU_load>100) CPU_load_overrun_count++; //keep track if there's an loop overrun
		while (sys_ticks < LOOPTIME);   // establishs the control loop time (sys_ticks updates every millisecond)
	    }//end of forever "for(;;)" loop
	
	return 0;
    }//end of main

void Set_PWM_D4(int error)  // scales the error to the PWM range; sets the direction on PTC3 
{
	if (error <0 )
	{//negative; set direction CCW
		gpo_c(3,0);
		error = -error;
	}
	else 
	{//positve, set direction CW
		gpo_c(3,1);		
	}
	if (error > PWM_D4_period){ //check if out of range
		error = PWM_D4_period;  //if so, set to max allowed
	}
	PWM_D4_pw = error;			//send new value to PWM
	PWM_timer_0_channel_4_pw(PWM_D4_pw);
	return;
}
void Set_PWM_A2(int error)  // scales the error to the PWM range; sets the direction on PTC0 
{
	if (error <0 )
	{//negative; set direction CCW
		gpo_c(0,0);
		error = -error;
	}
	else 
	{//positve, set direction CW
		gpo_c(0,1);		
	}
	if (error > PWM_A2_period){ //check if out of range
		error = PWM_A2_period;  //if so, set to max allowed
	}
	PWM_A2_pw = error;			//send new value to PWM
	PWM_timer_2_channel_1_pw(PWM_A2_pw);
	return;
}
void Check_Keypad(void){    
	//checks keypad and implements simple protocol:
	//   '0' - '9' are buffered until 'C' (for Kp) and 'D' for Kd) are pressed, the gains are updated and displayed
	//   'A' sends raw trimpot value to the screen
	//	 'B' sends raw Chan 12 value to the screen
lastkey = keystroke;
keystroke=getKey();
if ((lastkey != 0)&& (keystroke == 0))//implements simple rising edge detection
	{
		switch (lastkey){
		
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		        keybuffer[kbindex]= lastkey;
				kbindex++;
				break;
		case 'A':		//shows raw adc result of trimpot
			sprintf(Disp_Buffer,"Trim=%4i",trimpot_result);
			setLocation(8,2);writeString (Disp_Buffer); //3rd line on display,indented
			break;
		case 'B':    	//shows raw adc result of PTB2
			sprintf(Disp_Buffer,"Ch12=%4i",adc_ch12_result);
			setLocation(8,3);writeString (Disp_Buffer); //4th line on display,indented		
			break;
		case 'C':		////completes and shows Kp 
			keybuffer[kbindex]= 0; //complete the string
			Kp = atoi(keybuffer);  //update the gain(p)
			kbindex =0;
			sprintf(Disp_Buffer,"Kp=%5i",Kp);
			setLocation(8,4);writeString (Disp_Buffer); //5th line on display,indented	
			break;
		case 'D':			////completes and shows Kd
			keybuffer[kbindex]= 0; //complete the string
			Kd = atoi(keybuffer);  //update the gain(d)
			kbindex =0;
			sprintf(Disp_Buffer,"Kd=%5i",Kd);
			setLocation(8,5);writeString (Disp_Buffer); //6th line on display,indented	
			break;
		default:
			
			setLocation(0,7);writeString ("try again:"); //8th line on display
			printChar(lastkey);//for testing
		 }//end of switch (keystroke)						
			
			
	}//end of if (lastkey...
return;
}
void Initialize_All(void){	
	pll_init(8000000,0,1,4,24,1);     //set CPU clock to 48MHz
   // Activate leds and PBs
    init_gpio_c(12,1);   //LED1 : C12
    init_gpio_c(13,1);   //LED2 : C13
   // init_gpio_c(16,1);   //LED3 : C16
   // init_gpio_c(17,1);   //LED4 : C17     
    
    gpo_c(12,1);
    gpo_c(13,1);
    gpo_c(16,1);
    gpo_c(17,1);
    
    init_gpio_c(4,0);   //PB1 : C4
    init_gpio_c(5,0);   //PB2 : C5
    init_gpio_c(6,0);   //PB3 : C6
    init_gpio_c(7,0);   //PB4 : C7
    

    init_adc0();
    initKey();
    initializeDisplay();
    syst_tick_init(); 
    
    init_gpio_c(0,1);   // this pin used to set direction of L298 motor controller, 
    gpo_c(0,0);			//paired with A2 PWm unit
    InitPWM_PORTA2_timer_2_channel_1(); //starting first PWM
    PWM_timer_2_channel_1_per(PWM_A2_period);  //5.33us * argument
    PWM_timer_2_channel_1_pw(PWM_A2_pw);       //5.33us * argument
 
    init_gpio_c(3,1);   // this pin used to set direction of L298 motor controller, 
    gpo_c(3,0);				//paired with D4 PWm unit
    InitPWM_PORTD4_timer_0_channel_4();  //starting 2nd PWM
    PWM_timer_0_channel_4_per(PWM_D4_period); //5.33us * argument
    PWM_timer_0_channel_4_pw(PWM_D4_pw);	  //5.33us * argument
    kbindex=0;
	Kp = 1;  //update the gain(p)
	sprintf(Disp_Buffer,"Kp=%5i",Kp);
	setLocation(8,4);writeString (Disp_Buffer); //5th line on display,indented	
	
	Kd = 1;  //update the gain(d)
	sprintf(Disp_Buffer,"Kd=%5i",Kd);
	setLocation(8,5);writeString (Disp_Buffer); //6th line on display,indented	
    return;
}
   
