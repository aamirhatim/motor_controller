#ifndef CURRENTCONTROL__H__
#define CURRENTCONTROL__H__

void current_control_init();
int get_pwm(int);
void set_gains(int, int);
int get_gains(char);

#endif
