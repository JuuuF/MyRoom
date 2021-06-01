/** ===========================================================================
  main_vars.h

  This header file contains the most important global variables and macros for
  the system. Intended to be included in the main file before any other local
  file. This way all following files have access to these variables and macros.
*/

#define NUM_LEDs 392      // LED count

#define EEPROM_SIZE 1     // bytes usesd in EEPROM

/* pin declaration */
#define POTI_M_PIN 34     // mod poti pin
#define POTI_B_PIN 35     // brightness poti pin
#define BTN_L_PIN 2       // left button pin
#define BTN_R_PIN 15      // right button pin
#define LED_PIN 22        // led output pin

#define FRAME_DELAY 25    // milliseconds per frame
#define FPS 1000 / FRAME_DELAY    // animation fps