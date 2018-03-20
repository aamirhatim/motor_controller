#ifndef ICONTROL__H__
#define ICONTROL__H__

static volatile float iEint = 0;
volatile float ival;
volatile int store_data = 0, itestval = 200;
// static int direction;
int pwm;
int speed;


void current_control_init();
int get_pwm();
void set_pwm(int);
void set_dir();
float pi_control(int, float);
void itest_reset();

#endif
