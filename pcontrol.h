#ifndef PCONTROL__H__
#define PCONTROL__H__

#define IMAX 1070
volatile int iref = 0;
int traj_array[2000]; // 1000/200 = 10 seconds of motor controls

void hold_reset();
void pos_control_init();
void set_deg(int);
void set_deg_ticks(int);
int get_deg();
int to_ticks(int);
int to_deg(int);
int pid_control(int, int);
void traj_reset();

#endif
