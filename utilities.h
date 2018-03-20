#ifndef UTILITIES__H__
#define UTILITIES__H__

enum mode_type{
  IDLE,
  PWM,
  ITEST,
  HOLD,
  TRACK
}mode;

float Kp[2], Ki[2], Kd[2];

enum mode_type get_mode();
void set_mode(enum mode_type);
void set_gains(float, float, float, int);
float get_gains(char, int);

#endif
