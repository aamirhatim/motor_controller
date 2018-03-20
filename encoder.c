// Aamir Husain
// ME 333 Final Project
// Encoder Library

#include "encoder.h"
#include <xc.h>

static int encoder_command(int read) {    // send a command to the encoder
  SPI3BUF = read;                         // send command
  while(!SPI3STATbits.SPIRBF) {           // wait for response
    ;
  }

  SPI3BUF;                                // ignore grbage transfer on first recieve
  SPI3BUF = 5;                            // write garbage, read will have data
  while(!SPI3STATbits.SPIRBF) {
    ;
  }

  return SPI3BUF;
}

int encoder_counts(void) {
  return encoder_command(1);
}

int encoder_reset(void) {
  return encoder_command(0);
}

int encoder_degrees(void) {
  int count = encoder_command(1);       // get current position
  int rot = count - MIDDLE;             // calculate difference between middle value and counter
  // int diff = rot%LINES_PER_REV;
  int degree = rot*360/LINES_PER_REV;   // convert the difference to degrees
  return degree;
}

void encoder_init(void) {     // SPI initializer
  SPI3CON = 0;                // stop and reset SPI3
  SPI3BUF;                    // read to clear thr rx recieve buffer
  SPI3BRG = 0x4;              // bit rate to 8MHz, SPI3BRG = 80000000/(2*desired)-1
  SPI3STATbits.SPIROV = 0;    // clear the overflow
  SPI3CONbits.MSTEN = 1;      // master mode
  SPI3CONbits.MSSEN = 1;      // slave select enable
  SPI3CONbits.MODE16 = 1;     // 16-bit mode
  SPI3CONbits.MODE32 = 0;
  SPI3CONbits.SMP = 1;        // sample @ end of the clock
  SPI3CONbits.ON = 1;         // turn SPI on
}
