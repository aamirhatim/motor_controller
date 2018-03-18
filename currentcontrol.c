#include "currentcontrol.h"
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
  IPC2bits.T2IP = 6;          // priority
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

int get_pwm(int speed) {
  return (PR3+1)*abs(speed)/100;
}

void set_gains(int kp, int ki) {
  Kp = kp;
  Ki = ki;
  return;
}

int get_gains(char gain_type) {
  if (gain_type == 'p') {
    return Kp;
  }
  else if (gain_type == 'i') {
    return Ki;
  }
}
