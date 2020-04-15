/*
 * mandelbrot_set.c
 * By Sam Newmarco
 * Prints the mandelbrot set to the an
 * OLED screen and allows user to zoom in
 * and move around.
 * 
 * Controls:
 * 
 *  *  7  4  1
 *  0  8  5  2
 *  #  9  6  3
 *  D  C  B  A
 * 
 * # = zoom in
 * D = zoom out
 * C = move left
 * A = move right
 * 6 = move up
 * B = move down.
 * 
 */
#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_gpio.h" /* include peripheral declarations */
#include "my_adc.h"
#include "my_ssd1306.h"
#include "my_keypad.h"
#include "my_Pixy2.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <my_UART.h>
#include <my_sys_ticks.h>

#define MAIN_TICK_DELAY 20

#define MAX_V_POINTS 256

// Define buttons:
#define Button1          gpi_c(4)
#define Button2          gpi_c(5)
#define Button3          gpi_c(6)
#define Button4          gpi_c(7)

/**************Global Variables Here************************/

unsigned short frameHeight;
unsigned short frameWidth;
unsigned short hardware;
uint8_t firmwareMajor;
uint8_t firmwareMinor;
unsigned short firmwareBuild;
int cameraFPS;

struct block pixy2;

struct velocity{
	uint16_t x_pos;
	uint16_t y_pos;
	unsigned long time;
	float x_vel;
	float y_vel;
	float t_vel;
	float angle;
	uint8_t frame;
	
};

struct velocity velocity_points[MAX_V_POINTS];
int velocity_index = 0; 

/**************Function Prototypes Here*********************/
int Init_Pixy2(void);
void Init_Everything(void);    

int get_velocity(int sigmap, float ppi);
int   x_pos;
int   y_pos;
float x_vel;
float y_vel;
float t_vel;
float angle;
int frame; 
unsigned long v_time;
int8_t test;


uint8_t y=0;
uint16_t ad=0;
int main(void) {
	
//**********  Initials   ***********************

	pll_init(8000000,0,1,4,24,1); //Use the crystal oscillator to drive the PLL
	
	Init_Everything();
    //Initialize internal timer
    syst_tick_init();
	test = 0;
	char start_string[] = "Starting...\n\r";
	UART0_Transmit_String(start_string);

	
	uint32_t t0 = sys_ticks;
	
	for(;;){
		test = get_velocity(1, 77.78);//Length of frame = 48.75 in. Pixels = 316. pixels per foot = 77.78
		if(test >= 0){
			x_pos = velocity_points[velocity_index-1].x_pos;
			y_pos = velocity_points[velocity_index-1].y_pos;
			x_vel = velocity_points[velocity_index-1].x_vel;
			y_vel = velocity_points[velocity_index-1].y_vel;
			t_vel = velocity_points[velocity_index-1].t_vel;
			angle = velocity_points[velocity_index-1].angle;
			v_time = velocity_points[velocity_index-1].time; 
			frame = velocity_points[velocity_index-1].frame; 
			char buf[128];
			sprintf(buf,"Time of Measurement = %lu\tframe = %d\n\rX Position = %d\tX Velocity = %3.1f\n\rY Position = %d\tY Velocity = %3.1f\n\rVelocity = %3.1f\tAngle = %3.1f\n\n\r", v_time, frame, x_pos, x_vel, y_pos, y_vel, t_vel, angle);
			UART0_Transmit_String(buf);
		}else if(test == -2 && velocity_index){
			int i;
			float aveX = 0, aveY = 0, aveVel = 0, aveAng = 0;
			float maxVel = 0, minVel = 1000, maxAng = 0, minAng = 1000;
			float sdVel = 0, sdAng = 0;
			for(i = 2; i < velocity_index-1; i++){
				//Calculate averages:
				aveX += velocity_points[velocity_index-i].x_vel;
				aveY += velocity_points[velocity_index-i].y_vel;
				aveAng += velocity_points[velocity_index-i].angle;
				aveVel += velocity_points[velocity_index-i].t_vel;
				
				//Find min/max:
				if(fabs(velocity_points[velocity_index-i].t_vel)>maxVel){
					maxVel = velocity_points[velocity_index-i].t_vel;
				}
				if(fabs(velocity_points[velocity_index-i].t_vel)<minVel){
					minVel = velocity_points[velocity_index-i].t_vel;
				}
				if(fabs(velocity_points[velocity_index-i].angle)>maxAng){
					maxAng = velocity_points[velocity_index-i].angle;
				}
				if(fabs(velocity_points[velocity_index-i].angle)<minAng){
					minAng = velocity_points[velocity_index-i].angle;
				}
			}
			aveX /= (velocity_index-3);
			aveY /= (velocity_index-3);
			aveVel /= (velocity_index-3);
			aveAng /= (velocity_index-3);
			for(i = 2; i < velocity_index-1; i++){
				//Standard Deviation 
				sdAng += pow((velocity_points[velocity_index-i].angle - aveAng),2);
				sdVel += pow((velocity_points[velocity_index-i].t_vel - aveVel),2);
			}
			sdAng = sqrt(sdAng/(velocity_index-3));
			sdVel = sqrt(sdVel/(velocity_index-3));
			char buf[512];
			sprintf(buf,"Average X Velocity = %3.1f\n\rAverage Y Velocity = %3.1f\n\rMax Velocity = %3.1f\tMin Velocity = %3.1f"
					"\n\rAverage Velocity = %3.1f\tVelocity Standard Deviation = %3.1f\n\rMax Angle = %3.1f\tMin Angle = %3.1f"
					"\n\rAverage Angle = %3.1f\tAngle Standard Deviation = %3.1f\n\n\r", aveX, aveY, maxVel, minVel, aveVel, sdVel,
					maxAng, minAng, aveAng, sdAng);
			UART0_Transmit_String(buf);
			velocity_index = 0; //Reset block
			while(1);
		}
		while(sys_ticks - t0 < 32);
		t0 = sys_ticks;
	}//End of forever loop.
	
	
	return 0;
}// End of main

int Init_Pixy2(void){
	unsigned long t0 = sys_ticks;
	while(sys_ticks-t0 < 5000){
		if(getResolution(&frameWidth, &frameHeight) == PIXY_RESULT_OK){
			return PIXY_RESULT_OK; 
		}
		unsigned long t1 = sys_ticks;
		while(sys_ticks - t1 < 5);
	}
	// timeout
	return PIXY_RESULT_TIMEOUT;
}//End of Init_Pixy2

int get_velocity(int sigmap, float ppd){
	static int miss_counter = 0;
	
	if(getBlocks(sigmap, 1, &pixy2) == PIXY_RESULT_OK){//Check if there is data
		if(velocity_index <= MAX_V_POINTS){
			miss_counter = 0;
			velocity_points[velocity_index].frame = pixy2.age;
			velocity_points[velocity_index].x_pos = pixy2.x_pos;
			velocity_points[velocity_index].y_pos = pixy2.y_pos; 
			velocity_points[velocity_index].time = sys_ticks;
			if(velocity_index>0){//Make sure your not on the first point
				int x_change = (int)velocity_points[velocity_index].x_pos-(int)velocity_points[velocity_index-1].x_pos;
				int y_change = (int)velocity_points[velocity_index].y_pos-(int)velocity_points[velocity_index-1].y_pos;
				int t_change = (int)velocity_points[velocity_index].time-(int)velocity_points[velocity_index-1].time;
				velocity_points[velocity_index].x_vel = (float)x_change/t_change*1000/ppd;
				velocity_points[velocity_index].y_vel = (float)y_change/t_change*1000/ppd;
				velocity_points[velocity_index].t_vel = sqrt(pow(velocity_points[velocity_index].x_vel, 2)+pow(velocity_points[velocity_index].y_vel, 2));
				if(velocity_points[velocity_index].y_vel == 0.000000){//Prevent devision by 0.
					velocity_points[velocity_index].angle = 90;
				}else if(velocity_points[velocity_index].x_vel == 0.000000){
					velocity_points[velocity_index].angle = 0;
				}
				else{
					velocity_points[velocity_index].angle = 57.296*atan(velocity_points[velocity_index].x_vel/velocity_points[velocity_index].y_vel);
					if(velocity_points[velocity_index].angle < 0){
						velocity_points[velocity_index].angle += 180;
					}
				}
				
			}
			velocity_index++;
			return velocity_index;
		}
		else{
			return -1;//out of memory 
		}
	}
	else{
		miss_counter++;
		if(miss_counter > 5){//After 5 calls with no data assume the object has left the screen. 
			return -2;//Object is gone
		}
		return -3;//Pixy does not see object
	}
}

void Init_Everything(void){
//initialize everything, including ports and variables
    
	pll_init(8000000, 0, 1, 4, 24, 1);
	
	/*//Initialize all LEDs.
    init_gpio_c(12,1);
    init_gpio_c(13,1);
    init_gpio_c(16,1);
    init_gpio_c(17,1);
    
    //Turn off all LEDs.
    gpo_c(12,1);
    gpo_c(13,1);
    gpo_c(16,1);
    gpo_c(17,1);
    
    //Initialize buttons.
    init_gpio_c(4,0);
    init_gpio_c(5,0);
    init_gpio_c(6,0);
    init_gpio_c(7,0);*/

	//initKey();
	
	init_adc0();
	

    
    init_I2C0_PTC8_9();
    UART0_Init();
    
    if(Init_Pixy2() != PIXY_RESULT_OK){
    	char fail_string[] = "Pixy2 failed to init";
    	UART0_Transmit_String(fail_string);
    	while(1);
    }
    setCameraBrightness(135);
    
   
    
}//End of Init_Everything.





