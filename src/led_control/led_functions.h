

//======================================================================
// single-pixel functions

#pragma once
#include "utils.h"

extern const int NUM_LEDs;
extern NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip;


/**---------------------------------------------------------------------
  addPixelColor

  Add a color to a pixel instead of overriding it.

  Params:
    int pixel                   pixel to write to.
    RgbwColor color             RBW color.
  
*/
void addPixelColor(int pixel, RgbwColor color) {
  RgbwColor current = strip.GetPixelColor(pixel);
  RgbwColor result = RgbwColor(
                       constrain(current.R + color.R, 0, 255),
                       constrain(current.G + color.G, 0, 255),
                       constrain(current.B + color.B, 0, 255),
                       constrain(current.W + color.W, 0, 255)
                     );
  strip.SetPixelColor(pixel, result);
}



//======================================================================
// multi-pixel functions



/**---------------------------------------------------------------------
  setSolid

  Set a block of pixels to a given color.
  This method will override previous values.

  Params:
    int start_point = 0         pixel index to start from.
    int end_point  = NUM_LEDs   pixel index of last pixel (non-inclusive).
    RgbwColor color             RGBW color.
*/

void setSolid(int start_point, int end_point, RgbwColor color) {
  for (int i = start_point; i < end_point; i++) {
    strip.SetPixelColor(i, color);
  }
}

void setSolid(int end_point, RgbwColor color) {
  for (int i = 0; i < end_point; i++) {
    strip.SetPixelColor(i, color);
  }
}

void setSolid(RgbwColor color) {
  for (int i = 0; i < NUM_LEDs; i++) {
    strip.SetPixelColor(i, color);
  }
}



/**---------------------------------------------------------------------
  addSolid

  Set a block of pixels to a given color.
  In contrast to setSolid(), this method adds the given pixel color
  to the already existing pixel.

  Params:
    int start_point = 0         pixel index to start from.
    int end_point  = NUM_LEDs   pixel index of last pixel (non-inclusive).
    RgbwColor color             RGBW color.
*/

void addSolid(int start_point, int end_point, RgbwColor color) {
  for (int i = 0; i < end_point - start_point; i++) {
    addPixelColor(start_point + i, color);
  }
}

void addSolid(int end_point, RgbwColor color) {
  for (int i = 0; i < end_point; i++) {
    addPixelColor(i, color);
  }
}

void addSolid(RgbwColor color) {
  for (int i = 0; i < NUM_LEDs; i++) {
    // TODO addPixelColor(color);
  }
}



/**---------------------------------------------------------------------
  addFloat

  Set a floating block of pixels to a given color.
  In contrast to addSolid(), this methos uses floating point boundaries.

  Params:
    float start_point           starting position.
    float end_pint              end position.
    RgbwColor color             RBGW color.
    bool add = true             add to or override existing colors.

*/

void addFloat(float start_point, float end_point, RgbwColor color, bool add) {

  // first segment
  float first_seg_length = min(end_point - start_point, start_point - (int) start_point);
  uint8_t dim = 255 * (1 - first_seg_length);
  if (add) {
    addPixelColor((int) start_point, RgbwColor(color.Dim(dim)));
  } else {
    strip.SetPixelColor((int) start_point, RgbwColor(color.Dim(dim)));
  }

  if (end_point - start_point < start_point - (int) start_point) {
    // segment only on one pixel
    return;
  }

  // middle segment
  if (add) {
    addSolid(ceil(start_point), floor(end_point), color);
  } else {
    setSolid(ceil(start_point), floor(end_point), color);
  }


  // last segment
  dim = 255 * (end_point - (int) end_point);
  RgbwColor res_color = color.Dim(dim);
  if (add) {
    addPixelColor((int) end_point, res_color);
  } else {
    strip.SetPixelColor((int) end_point, res_color);
  }

}

void addFloat(float start_point, float end_point, RgbwColor color) {
  addFloat(start_point, end_point, color, true);
}



//======================================================================
// all-pixel functions


/**---------------------------------------------------------------------
  fadeToBlackBy
   
   Fade all lights to black by some steps.

   Params:
    uint8_t amount              amount of steps to reduce color values.
*/
void fadeToBlackBy(uint8_t amount) {
  for (int i = 0; i < NUM_LEDs; i++) {
    RgbwColor current = strip.GetPixelColor(i);
    RgbwColor new_color = RgbwColor(
                            max(0, current.R - amount),
                            max(0, current.G - amount),
                            max(0, current.B - amount),
                            max(0, current.W - amount)
                          );
    strip.SetPixelColor(i, new_color);
  }
}



/**---------------------------------------------------------------------
  fadeToBlackRandom
  
  Fade all lights to black by a random amount each. Lower and upper bounds
  can be given.

  Params:
    uint8_t lower = 0           lower boundary.
    uint8_t upper = 128         upper boundary.
*/

void fadeToBlackRandom(int lower, int upper) {
  for (int i = 0; i < NUM_LEDs; i++) {
    RgbwColor current = strip.GetPixelColor(i);
    int amount = random(lower, upper);
    RgbwColor new_color = RgbwColor(
                            max(0, current.R - amount),
                            max(0, current.G - amount),
                            max(0, current.B - amount),
                            max(0, current.W - amount)
                          );
    strip.SetPixelColor(i, new_color);
  }
}

void fadeToBlackRandom() {
  fadeToBlackRandom(0, 128);
}



/**---------------------------------------------------------------------
  multByFactor
  
  Multiply all lights by some factor.

  Params:
    float factor                multiplying factor.
*/

void multByFactor(float factor) {
  for (int i = 0; i < NUM_LEDs; i++) {
    RgbwColor color = strip.GetPixelColor(i);
    strip.SetPixelColor(i, RgbwColor(
                          constrain(color.R * factor, 0, 255),
                          constrain(color.G * factor, 0, 255),
                          constrain(color.B * factor, 0, 255),
                          constrain(color.W * factor, 0, 255)
                        ));
  }
}



/**---------------------------------------------------------------------
  clear_strip
  
  Turn off all lights.
*/
void clear_strip() {
  for (uint8_t i = 0; i < NUM_LEDs; i++) {
    strip.SetPixelColor(i, RgbwColor(0));
  }
}


/**---------------------------------------------------------------------
  show
  
  Draw leds to strip with respect to MAX_MILLIAMPS.
  Led colors are dimmed to keep current draw under MAX_MILLIAMPS.

  Params:
    bool safe_mode = true       don't display if something went wrong.
                                NOTE: will draw 12.5 mA.
  
*/
void show(bool safe_mode) {
  int draw = calculateMilliAmps();
  if (draw > MAX_MILLIAMPS) {
    // dimm all leds
    float fac = (float) MAX_MILLIAMPS / draw;
    multByFactor(fac * 0.95); // just to be sure
  }

  // warning blink if something goes wrong
  if (safe_mode && calculateMilliAmps() > MAX_MILLIAMPS) {
    clear_strip();
    for (int i = 0; i < 3; i++) {
      setSolid(0,5,RgbwColor(32,0,0,0));
      strip.Show();
      delay(10);
      setSolid(0,5,RgbwColor(0));
      strip.Show();
      delay(10);
    }
  }
  strip.Show();
}

void show() {
  show(true);
}

//======================================================================
// special-pixel functions



/**---------------------------------------------------------------------
  randomSparkles

  Randomly turn a pixel color to equally bright white pixel.
*/
void randomSparkles() {
  if (random(5) == 0) {
    int index = random(NUM_LEDs);
    while (strip.GetPixelColor(index).R == 0 &&
           strip.GetPixelColor(index).G == 0 &&
           strip.GetPixelColor(index).B == 0 &&
           strip.GetPixelColor(index).W == 0) {
      index++;
      if (index == NUM_LEDs)
        return;
    }
    int sparkle_color = maxColorVal(strip.GetPixelColor(index));
    strip.SetPixelColor(index, RgbwColor(0, 0, 0, sparkle_color));
  }
}
