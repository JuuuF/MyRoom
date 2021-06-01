
/** ===========================================================================
  led_functions.h

  This file contains basic led functions.
  TODO: add buffer compatibility to all functions.
*/

#pragma once
#include "utils.h"

extern NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip;

/* ========================================================================= */
/* single-pixel functions */

/**----------------------------------------------------------------------------
  setPixel

  Set a color to a pixel. Just to be consistent with the functions.

  Parameters:
  uint16_t pixel                    pixel to write to
  RgbwColor color                   RGBW color
*/
inline void setPixel(uint16_t pixel, RgbwColor color) {
  strip.SetPixelColor(pixel, color);
}



/**---------------------------------------------------------------------
  addPixel

  Add a color to a pixel instead of overriding it.

  Parameters:
    uint16_t pixel                  pixel to write to
    RgbwColor color                 RGBW color
*/
void addPixel(uint16_t pixel, RgbwColor color) {
  RgbwColor current = strip.GetPixelColor(pixel);
  RgbwColor result = RgbwColor(
                       constrain(current.R + color.R, 0, 255),
                       constrain(current.G + color.G, 0, 255),
                       constrain(current.B + color.B, 0, 255),
                       constrain(current.W + color.W, 0, 255)
                     );
  setPixel(pixel, result);
}



/* ========================================================================= */
/* multi-pixel functions */

/**----------------------------------------------------------------------------
  setSolid

  Set a block of pixels to a given color.
  This function will override previous values.
  If no start point is given, 0 is assumed.
  If neither start nor end are given, the whole strip is affected.

  Parameters:
    uint16_t start_point            pixel index to start from
    uint16_t end_point              pixel index of last pixel (exclusive)
    RgbwColor color                 RGBW color
*/

void setSolid(uint16_t start_point, uint16_t end_point, RgbwColor color) {
  for (uint16_t i = start_point; i < end_point; i++) {
    setPixel(i, color);
  }
}

void setSolid(uint16_t end_point, RgbwColor color) {
  setSolid(0, end_point, color);
}

void setSolid(RgbwColor color) {
  setSolid(0, NUM_LEDs, color);
}



/**----------------------------------------------------------------------------
  addSolid

  Set a block of pixels to a given color.
  In contrast to setSolid(), this function adds the given pixel color to the
  already existing pixel.
  If no start point if given, 0 is assumed.
  If neither start nor end are given, the whole strip is affected.

  Parameters:
    uint16_t start_point            pixel index to start from
    uint16_t end_point              pixel index of last pixel (non-inclusive)
    RgbwColor color                 RGBW color
*/

void addSolid(uint16_t start_point, uint16_t end_point, RgbwColor color) {
  for (uint16_t i = start_point; i < end_point; i++) {
    addPixel(i, color);
  }
}

void addSolid(uint16_t end_point, RgbwColor color) {
  addSolid(0, end_point, color);
}

void addSolid(RgbwColor color) {
  addSolid(0, NUM_LEDs, color);
}



/**----------------------------------------------------------------------------
  setFloat

  Set a floating block of pixels to a given color, overriding other colors.
  In contrast to setSolid(), this function uses floating point boundaries.
  If no start point is given, 0 is assumed.
  If neither start nor end are given, the whole strip is affected.

  Parameters:
    float start_point               starting position
    float end_point                 end position
    RgbwColor color                 RBGW color
*/

void setFloat(float start_point, float end_point, RgbwColor color) {
  start_point = constrain(start_point, 0, NUM_LEDs);
  end_point   = constrain(end_point,   0, NUM_LEDs);

  // first segment
  float first_seg_length = min(end_point - start_point, start_point - (uint8_t) start_point);
  uint8_t dim = 255 * (1 - first_seg_length);
  setPixel((uint8_t) start_point, RgbwColor(color.Dim(dim)));

  if (end_point - start_point < start_point - (uint8_t) start_point) {
    // segment only on one pixel
    return;
  }

  // middle segment
  setSolid(ceil(start_point), floor(end_point), color);


  // last segment
  dim = 255 * (end_point - (uint8_t) end_point);
  RgbwColor res_color = color.Dim(dim);
  setPixel((uint8_t) end_point, res_color);
}

void setFloat(float end_point, RgbwColor color) {
  setFloat(0, end_point, color);
}

void setFloat(RgbwColor color) {
  // this is essentially setSolid
  setSolid(0, NUM_LEDs, color);
}



/**----------------------------------------------------------------------------
  addFloat

  Set a floating block of pixels to a given color.
  In contrast to addSolid(), this methos uses floating point boundaries.
  If no start point is given, 0 is assumed.
  If neither start nor end are given, the whole strip is affected.

  Parameters:
    float start_point               starting position
    float end_point                 end position
    RgbwColor color                 RBGW color
*/
void addFloat(float start_point, float end_point, RgbwColor color) {

  // first segment
  float first_seg_length = min(end_point - start_point, start_point - (uint8_t) start_point);
  uint8_t dim = 255 * (1 - first_seg_length);
  addPixel((uint8_t) start_point, RgbwColor(color.Dim(dim)));

  if (end_point - start_point < start_point - (uint8_t) start_point) {
    // segment only on one pixel
    return;
  }

  // middle segment
  addSolid(ceil(start_point), floor(end_point), color);


  // last segment
  dim = 255 * (end_point - (uint8_t) end_point);
  RgbwColor res_color = color.Dim(dim);
  addPixel((uint8_t) end_point, res_color);
}

void addFloat(float end_point, RgbwColor color) {
  addFloat(0, end_point, color);
}

void addFloat(RgbwColor color) {
  // this is essentially addSolid
  addSolid(0, NUM_LEDs, color);
}



/* ========================================================================= */
/* all-pixel functions */

/**----------------------------------------------------------------------------
  fadeToBlackBy

   Fade all lights to black by some steps.
   If no buffer is given or the buffer is NULL, the strip active strip is
   modified.

   Parameters:
    uint8_t amount                  amount of steps to reduce color values
    RgbwColor* buffer               scene to fade instead of led strip
*/
void fadeToBlackBy(uint8_t amount) {
  for (uint8_t i = 0; i < NUM_LEDs; i++) {
    RgbwColor current = strip.GetPixelColor(i);
    RgbwColor new_color = RgbwColor(
                            max(0, current.R - amount),
                            max(0, current.G - amount),
                            max(0, current.B - amount),
                            max(0, current.W - amount)
                          );
    setPixel(i, new_color);
  }
}

void fadeToBlackBy(uint8_t amount, RgbwColor* buffer) {
  if (buffer == NULL)
    return fadeToBlackBy(amount);

  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    buffer[i].R = max(0, buffer[i].R - amount);
    buffer[i].G = max(0, buffer[i].G - amount);
    buffer[i].B = max(0, buffer[i].B - amount);
    buffer[i].W = max(0, buffer[i].W - amount);
  }
}



/**----------------------------------------------------------------------------
  fadeToBlackRandom

  Fade all lights to black by a random amount each. Lower and upper bounds
  can be given. If not, they are defaulted to 0 and 128.

  Parameters:
    uint8_t lower                   lower boundary
    uint8_t upper                   upper boundary
*/

void fadeToBlackRandom(uint8_t lower, uint8_t upper) {
  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    RgbwColor current = strip.GetPixelColor(i);
    uint8_t amount = random(lower, upper);
    RgbwColor new_color = RgbwColor(
                            max(0, current.R - amount),
                            max(0, current.G - amount),
                            max(0, current.B - amount),
                            max(0, current.W - amount)
                          );
    setPixel(i, new_color);
  }
}

void fadeToBlackRandom() {
  fadeToBlackRandom(0, 128);
}



/**----------------------------------------------------------------------------
  multByFactor

  Multiply all lights by some factor.

  Parameters:
    float factor                    multiplication factor
*/

void multByFactor(float factor) {
  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    RgbwColor color = strip.GetPixelColor(i);
    setPixel(i, RgbwColor(
                          constrain(color.R * factor, 0, 255),
                          constrain(color.G * factor, 0, 255),
                          constrain(color.B * factor, 0, 255),
                          constrain(color.W * factor, 0, 255)
                        ));
  }
}



/**----------------------------------------------------------------------------
  clear_strip

  Turn off all lights.
*/
void clear_strip() {
  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    setPixel(i, RgbwColor(0));
  }
}


/**----------------------------------------------------------------------------
  show

  Show the LED strip.
  LED colors are dimmed to keep current draw under MAX_MILLIAMPS.
  If MAX_MILLIAMPS was not set or is set to INFINITY, this check will be
  skipped.
*/
void show() {

  if (MAX_MILLIAMPS != INFINITY) {
    int draw = calculate_milliamps();
    if (draw > MAX_MILLIAMPS) {
      // dimm all leds
      float fac = (float) MAX_MILLIAMPS / draw;
      multByFactor(fac * 0.95); // just to be sure
    }
  }

  strip.Show();
}
