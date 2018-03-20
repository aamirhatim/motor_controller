#include "pcontrol.h"
#include <xc.h>

void pos_control_init() {
  // Use pin D10 as a digital ouput pin
  TRISDbits.TRISD10 = 0;       // pin RD11 (D11) configured as digital output
  LATDbits.LATD10 = 1;         // set digital out to low

  // TIMER4 for 200Hz position control loop
  PR4 = 49999;                // period = 80M/200 = 400000/8 = 50000
  TMR4 = 0;                   // set Timer4 count to 0
  T4CONbits.TCKPS = 0b011;    // prescaler of 8
  T4CONbits.TGATE = 0;
  T4CONbits.ON = 1;           // turn Timer4 on
  IPC4bits.T4IP = 6;          // priority
  IPC4bits.T4IS = 0;          // subpriority 0
  IFS0bits.T4IF = 0;          // set interrupt flag to 0
  IEC0bits.T4IE = 1;          // enable interrupt
}
