#ifndef PCONTROL__H__
#define PCONTROL__H__

#define IMAX 1070
volatile int iref = 0;

void pos_control_init();
void set_deg(int);
int get_deg();
int to_ticks(int);
int pid_control(int, int);

#endif
