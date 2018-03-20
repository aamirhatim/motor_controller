// Aamir Husain
// ME 333: Final Project

#include "NU32.h"
#include "encoder.h"
#include "utilities.h"
#include "isense.h"
#include "icontrol.h"
#include <stdio.h>

#define BUF_SIZE 200
#define PLOTPTS 100

int i;
float kp, ki, kd;

static volatile int ADCarray[PLOTPTS];
static volatile int REFarray[PLOTPTS];
void __ISR(_TIMER_2_VECTOR, IPL5SOFT) CurrentControl(void) {
  static int control;

  switch (get_mode()) {
    case IDLE: {
      OC1RS = 0;
      break;
    }

    case PWM: {
      OC1RS = get_pwm();
      break;
    }

    case ITEST: {
      static volatile int icount;

      // Read current value from ADC pin
      ival = ADC_mA();
      control = pi_control(itestval, ival);

      set_dir((int) control);
      ADCarray[icount] = (int) ival;
      REFarray[icount] = itestval;
      OC1RS = (unsigned int) abs(control/100)*PR3;

      if (icount == 25 || icount == 50 || icount == 75) {
        itestval = itestval*(-1);
        icount++;
      }
      else if (icount >= 99) {
        set_mode(IDLE);
        store_data = 0;
        icount = 0;
      }
      else {
        icount++;
      }

      break;
    }
  }

  IFS0bits.T2IF = 0;          // reset interrupt flag
}

void __ISR(_TIMER_4_VECTOR, IPL6SOFT) PositionControl(void) {
  LATDINV = 0x400;

  IFS0bits.T4IF = 0;          // reset interrupt flag
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
  pos_control_init();
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
        sprintf(buffer, "%f\r\n", ADC_mA());
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
        int spd;
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%d", &spd);
        set_dir(spd);
        set_pwm(spd);
        set_mode(PWM);
        break;
      }

      // Set current gains
      case 'g': {
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%f", &kp);
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%f", &ki);
        set_gains(kp, ki, 0, 0);
        break;
      }

      // Get current gains
      case 'h': {
        sprintf(buffer, "%f\r\n", get_gains('p', 0));
        NU32_WriteUART3(buffer);
        sprintf(buffer, "%f\r\n", get_gains('i', 0));
        NU32_WriteUART3(buffer);
        break;
      }

      // Set position gains
      case 'i': {
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%f", &kp);
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%f", &ki);
        NU32_ReadUART3(buffer, BUF_SIZE);
        sscanf(buffer, "%f", &kd);
        set_gains(kp, ki, kd, 1);
        break;
      }

      // Get position gains
      case 'j': {
        sprintf(buffer, "%f\r\n", get_gains('p', 1));
        NU32_WriteUART3(buffer);
        sprintf(buffer, "%f\r\n", get_gains('i', 1));
        NU32_WriteUART3(buffer);
        sprintf(buffer, "%f\r\n", get_gains('d', 1));
        NU32_WriteUART3(buffer);
        break;
      }

      // Test current gains
      case 'k': {
        itest_reset();
        set_mode(ITEST);
        while (store_data) {
          ;
        }

        for (i = 0; i < PLOTPTS; i++) {
          sprintf(buffer, "%d %d\r\n", REFarray[i], ADCarray[i]);
          NU32_WriteUART3(buffer);
        }
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
