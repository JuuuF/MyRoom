
/** ===========================================================================
  utils.h

  This file contains some util functions such as LED getter and setter and
  functions checking LED states (e.g. buffer_empty).
*/

#pragma once
#include "led_functions.h"

extern float MAX_MILLIAMPS;
extern NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip;

/* external input variables */
extern const float INPUT_SMOOTHING;
extern float BRIGHTNESS, MOD;
extern bool LEFT_BUTTON, RIGHT_BUTTON;

#define ARRAY_SIZE(arr) (uint16_t) (*(&arr + 1) - arr)

using namespace std;


/* ========================================================================= */
/* led getter functions */

/**----------------------------------------------------------------------------
  maxColorVal

  Get the maximum color value of a given color.

  Parameters:
  RgbwColor color             color.

  Returns:
  int                         max of all color values.
*/
inline uint8_t maxColorVal(RgbwColor color) {
  return max(max(color.R, color.G), max(color.B, color.W));
}



/**----------------------------------------------------------------------------
  buffer_empty

  Determine whether the given buffer is empty. If no buffer is given, the led
  strip is assumed.

  Parameters:
    RgbwColor* buffer (optional)    buffer of size NUM_LEDs.

  Returns:
    true                      if all colors are black,
    false                     otherwise.
*/
bool buffer_empty(RgbwColor* buffer) {
  for (int i = 0; i < NUM_LEDs; i++) {
    if (buffer[i].R != 0 || buffer[i].G != 0 || 
        buffer[i].B != 0 || buffer[i].W != 0)
      return false;
  }
  return true;
}

bool buffer_empty() {
  for (int i = 0; i < NUM_LEDs; i++) {
    RgbwColor col = strip.GetPixelColor(i);
    if (col.R != 0 || col.G != 0 || 
        col.B != 0 || col.W != 0)
      return false;
  }
  return true;
}



/**----------------------------------------------------------------------------
  calculateMilliAmps

  Calculate the milliamps used to display a given buffer. If no buffer is
  given, the current led strip is assumed.

  NOTE: maxes out at about 65A. Just keep that in mind.

  Parameters:
    RgbwColor* buffer (optional)    buffer of size NUM_LEDs

  Returns:
    int                         current drawn in milliamps.
*/
uint16_t calculate_milliamps(RgbwColor* buffer) {
  uint16_t total = 0;
  float temp;
  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    // accumulate in float for that extra precision
    temp = 0;
    // max. 20 mAh per led
    temp += (buffer[i].R * 20) / 255.0;
    temp += (buffer[i].G * 20) / 255.0;
    temp += (buffer[i].B * 20) / 255.0;
    temp += (buffer[i].W * 20) / 255.0;
    total += temp;
  }
  return total;
}

uint16_t calculate_milliamps() {
  uint16_t total = 0;
  float temp;
  RgbwColor col;
  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    // accumulate in float for that extra precision
    temp = 0;
    col = strip.GetPixelColor(i);
    // max. 20 mAh per led
    temp += (col.R * 20) / 255.0;
    temp += (col.G * 20) / 255.0;
    temp += (col.B * 20) / 255.0;
    temp += (col.W * 20) / 255.0;
    total += temp;
  }
  return total;
}



/* ========================================================================= */
// led setter functions

/**----------------------------------------------------------------------------
  setMaxMilliamps

  Set maximum milliamps to draw.

  Parameters:
    float val                   max current to draw in milliamps.

  Sets:
    float MAX_MILLIAMPS             max milliamps to draw.
*/
extern float MAX_MILLIAMPS;
void setMaxMilliamps(float val) {
  MAX_MILLIAMPS = val;
}



/* ========================================================================= */
// other led functions

/**----------------------------------------------------------------------------
  Hsvw2Rgbw

  Convert HSVW color to RBGW color.

  Source: https://www.codespeedy.com/hsv-to-rgb-in-cpp/

  Parameters:
    float H: [0,360]                hue.
    float S: [0,1]                  saturation.
    float V: [0,1]                  value.
    float W: [0,1]                  white component.

  Returns:
    RgbwColor                       black     , if invalid inputs
                                    rgb color , else
*/
RgbwColor Hsvw2Rgbw(float H, float S, float V, float W) {

  H = constrain(H, 0, 360);
  S = constrain(S, 0, 1);
  V = constrain(V, 0, 1);
  W = constrain(W, 0, 1);

  float C = S * V;
  float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  float m = V - C;
  float r, g, b;
  if (H >= 0 && H < 60) {
    r = C, g = X, b = 0;
  }
  else if (H >= 60 && H < 120) {
    r = X, g = C, b = 0;
  }
  else if (H >= 120 && H < 180) {
    r = 0, g = C, b = X;
  }
  else if (H >= 180 && H < 240) {
    r = 0, g = X, b = C;
  }
  else if (H >= 240 && H < 300) {
    r = X, g = 0, b = C;
  }
  else {
    r = C, g = 0, b = X;
  }
  uint8_t R = (r + m) * 255;
  uint8_t G = (g + m) * 255;
  uint8_t B = (b + m) * 255;

  return RgbwColor(R, G, B, (uint8_t) (W * 255));
}


/* ========================================================================= */
// other functions

/**----------------------------------------------------------------------------
  update_inputs

  Update all inputs.

  Sets:
    BRIGHTNESS                      interpolated brightness:  [0,1]
    MOD                             interpolated mod:         [0,1]
    LEFT_BUTTON                     true if left  button is pressed
    RIGHT_BUTTON                    true if right button is pressed
*/
// TODO: implement non-linear brightness calculation
void update_inputs() {
  // button inputs
  LEFT_BUTTON  = digitalRead(BTN_L_PIN);
  RIGHT_BUTTON = digitalRead(BTN_R_PIN);

  // poti inputs
  float reading_brightness = (analogRead(POTI_B_PIN)/4096.0);
  float reading_mod = (analogRead(POTI_M_PIN)/4096.0);
  BRIGHTNESS  = INPUT_SMOOTHING * BRIGHTNESS  + (1 - INPUT_SMOOTHING) * reading_brightness;
  MOD         = INPUT_SMOOTHING * MOD         + (1 - INPUT_SMOOTHING) * reading_mod;
}
