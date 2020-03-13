#ifndef my_PWM_H_
#define my_PWM_H_

#include "derivative.h"

//Enter Period and Pulse Width in uSec

void InitPWM_PORTA2_timer_2_channel_1(void);     
void PWM_timer_2_channel_1_per(int per);
void PWM_timer_2_channel_1_pw(int pw);

void InitPWM_PORTD4_timer_0_channel_4(void);
void PWM_timer_0_channel_4_per(int per);
void PWM_timer_0_channel_4_pw(int pw);

void InitPWM_PORTD0_timer_0_channel_0(void);
void PWM_timer_0_channel_0_per(int per);
void PWM_timer_0_channel_0_pw(int pw);

void InitPWM_PORTE20_timer_1_channel_0(void);
void PWM_timer_1_channel_0_per(int per);
void PWM_timer_1_channel_0_pw(int pw);


#endif //my_PWM_H_
