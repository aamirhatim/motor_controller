// Aamir Husain
// ME 333: Final Project

#include "NU32.h"
#include "encoder.h"
#include "utilities.h"
#include "isense.h"
#include "currentcontrol.h"
#include <stdio.h>

#define BUF_SIZE 200

static int speed;
static int pwm;

void __ISR(_TIMER_2_VECTOR, IPL6SOFT) CurrentControl(void) {
  switch (get_mode()) {
    case IDLE: {
      OC1RS = 0;
      break;
    }

    case PWM: {
      if (speed < 0) {
        LATDSET = 0x800;
      }
      else {
        LATDCLR = 0x800;
      }

      OC1RS = pwm;
      break;
    }

    case ITEST: {
      break;
    }
  }

  IFS0bits.T2IF = 0;          // reset interrupt flag
}

int main() {
  char buffer[BUF_SIZE];
  NU32_Startup();
  encoder_init();       // initialize SPI3
  set_mode(IDLE);          // enter IDLE mode on sartup
  ADC_init();           // Enable ADC pin 1 (AN1)
  NU32_LED1 = 1;
  NU32_LED2 = 1;

  __builtin_disable_interrupts();
  current_control_init();
  __builtin_enable_interrupts();

  while(1) {
    NU32_ReadUART3(buffer, BUF_SIZE);
    NU32_LED2 = 1;

    switch (buffer[0]) {
      // Read current sensor (ADC counts)
      case 'a': {
        sprintf(buffer, "%d\r\n", (int) ADC_Sample_Convert());
        NU32_WriteUART3(buffer);
        break;
      }

      // Read current sensor (mA)
      case 'b': {
        sprintf(buffer, "%d\r\n", ADC_mA());
        NU32_WriteUART3(buffer);
        break;
      }

      // Read encoder (counts)
      case 'c': {
        encoder_counts();
        sprintf(buffer, "%d\r\n", encoder_counts());
        NU32_WriteUART3(buffer);  // Send encoder count to client
        break;
      }

      // Read encoder (degrees)
      case 'd': {
        encoder_counts();
        sprintf(buffer, "%d\r\n", encoder_degrees());
        NU32_WriteUART3(buffer);  // Send encoder count to client
        break;
      }

      // Reset encoder
      case 'e': {
        encoder_reset();
        sprintf(buffer, "%d\r\n", encoder_reset());
        NU32_WriteUART3(buffer);  // Send encoder count to client
        break;
      }

      // Set PWM
      case 'f': {
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%d", &speed);
        pwm = get_pwm(speed);
        set_mode(PWM);
        break;
      }

      // Set current gains
      case 'g': {
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%d", &Kp);
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%d", &Ki);
        break;
      }

      // Get current gains
      case 'h': {
        sprintf(buffer, "%d", get_gains('p'));
        NU32_WriteUART3(buffer);
        sprintf(buffer, "%d", get_gains('i'));
        NU32_WriteUART3(buffer);
        break;
      }

      // Unpower motor
      case 'p': {
        set_mode(IDLE);
        break;
      }

      // Quit
      case 'q': {
        set_mode(IDLE);
        break;
      }

      // Get mode
      case 'r': {
        sprintf(buffer, "%d\r\n", get_mode());
        NU32_WriteUART3(buffer);
        break;
      }

      default: {
        NU32_LED2 = 0;
        break;
      }
    }
  }

  return 0;
}
