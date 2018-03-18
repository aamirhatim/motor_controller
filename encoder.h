#ifndef ENCODER__H__
#define ENCODER__H__

// encoder_command(int) is private, so it does not appear here

void encoder_init();          // initialize the encoder module

int encoder_counts();          // read the encoder, in ticks

#endif

