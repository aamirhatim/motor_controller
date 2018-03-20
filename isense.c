#include "isense.h"
#include <xc.h>

#define VCC 3.3
#define ADC_RES 1024
#define SAMPLE_TIME 10

void ADC_init() {
  // Enable AN1 for analog input
  AD1PCFGbits.PCFG0 = 0;
  AD1CON3bits.ADCS = 2;
  AD1CON1bits.ADON = 1;
}

unsigned int ADC_Sample_Convert() {
  unsigned int elapsed = 0, finish_time = 0;
  AD1CHSbits.CH0SA = 0;                         // connect AN0 to MUXA for sampling
  AD1CON1bits.SAMP = 1;                         // start sampling
  elapsed = _CP0_GET_COUNT();
  finish_time = elapsed + SAMPLE_TIME;

  while (_CP0_GET_COUNT() < finish_time) {
    ;
  }

  AD1CON1bits.SAMP = 0;                         // stop sampling after specified time
  while (!AD1CON1bits.DONE) {
    ;
  }
  return ADC1BUF0;                              // return result
}

float ADC_mA() {
  unsigned int adcval = ADC_Sample_Convert();
  return 1.97*adcval - 1010.87;
}
