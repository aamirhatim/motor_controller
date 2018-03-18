#ifndef UTILITIES__H__
#define UTILITIES__H__

enum mode_type{
  IDLE,
  PWM,
  ITEST,
  HOLD,
  TRACK
}mode;

enum mode_type get_mode();
void set_mode(enum mode_type);

#endif
