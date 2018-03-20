#ifndef CURRENTCONTROL__H__
#define CURRENTCONTROL__H__

static volatile float Eint = 0;
volatile float ival;
volatile int store_data = 0, itestval = 200;
// static int direction;
int pwm;
int speed;


void current_control_init();
void set_speed(int);
int get_speed();
int get_pwm();
void set_pwm(int);
int get_dir();
void set_dir();
float get_gains(char);
void set_gains(float, float);
float pi_control(int, float);
void itest_reset();

#endif
