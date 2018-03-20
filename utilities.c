#include "utilities.h"

enum mode_type get_mode() {
  return mode;
}

void set_mode(enum mode_type m) {
  mode = m;
}

void set_gains(float kp, float ki, float kd, int controller) {
  Kp[controller] = kp;
  Ki[controller] = ki;
  Kd[controller] = kd;
  return;
}

float get_gains(char gain_type, int controller) {
  if (gain_type == 'p') {
    return Kp[controller];
  }
  else if (gain_type == 'i') {
    return Ki[controller];
  }
  else if (gain_type == 'd') {
    return Kd[controller];
  }
}
