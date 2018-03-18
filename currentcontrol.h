#ifndef CURRENTCONTROL__H__
#define CURRENTCONTROL__H__

static volatile int Kp = 0, Ki = 0, Eint = 0;

void current_control_init();
int get_pwm(int);
void set_gains(int, int);
int get_gains(char);

#endif
