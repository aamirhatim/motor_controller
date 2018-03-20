#include "icontrol.h"
// #include "NU32.h"
#include "utilities.h"
#include <xc.h>
#include <stdlib.h>

void current_control_init() {
  // Configure pin RB1 (I/O pin D11)
  TRISDbits.TRISD11 = 0;       // pin RD11 (D11) configured as digital output
  LATDbits.LATD11 = 0;         // set digital out to low

  // TIMER2: 5kHz for the ISR
  PR2 = 15999;                // period = 80M/5k = 16000
  TMR2 = 0;                   // set Timer2 count to 0
  T2CONbits.TCKPS = 0;        // prescaler of 1
  T2CONbits.TGATE = 0;
  T2CONbits.ON = 1;           // turn Timer2 on
  IPC2bits.T2IP = 5;          // priority
  IPC2bits.T2IS = 0;          // subpriority 0
  IFS0bits.T2IF = 0;          // set interrupt flag to 0
  IEC0bits.T2IE = 1;          // enable interrupt

  // TIMER3: 20kHz for PWM
  T3CONbits.TCKPS = 0;        // prescaler of 1
  PR3 = 3999;                 // period = 80M/20k = 4000
  TMR3 = 0;                   // set Timer3 count to 0
  OC1CONbits.OCM = 0b110;     // PWM mode without fault pin
  OC1CONbits.OCTSEL = 1;      // set OC1 to use Timer3
  OC1RS = 1000;               // duty cycle = OC1RS/(PR3+1) = 25%
  OC1R = 1000;
  T3CONbits.ON = 1;           // turn on Timer3
  OC1CONbits.ON = 1;          // turn on OC1
}

void set_pwm(int p) {
  pwm = (PR3)*abs(p)/100;
}

int get_pwm() {
  return pwm;
}

void set_dir(int s) {
  if (s < 0) {
    LATDSET = 0x800;
  }
  else if (s > 1) {
    LATDCLR = 0x800;
  }
}

void itest_reset() {
  store_data = 1;
  iEint = 0;
  itestval = 200;
  ival = 0;
}

float pi_control(int refval, float realval) {
  float err = 0;
  float u = 0;

  err = refval - realval;
  iEint += err;

  u = (Kp[0]*err) + (Ki[0]*iEint);

  if (u > 100.0) {
    u = 100.0;
  }
  else if (u < -100.0) {
    u = -100.0;
  }
  return u;
}
