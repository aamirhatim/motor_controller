#ifndef ENCODER__H__
#define ENCODER__H__

// encoder_command(int) is private, so it does not appear here
#define LINES_PER_REV 1792                // 448 lines on encoder, 4x multiplier
#define MIDDLE 32768

void encoder_init();          // initialize the encoder module

int encoder_counts();          // read the encoder, in ticks
int encoder_degrees();

#endif
