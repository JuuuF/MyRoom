//----------------------------------------------------------------
// led functions


/**
   Fill a solid strip from @code{first} to @code{last} (non-inclusive).
*/
void fillSolid(int first, int last, RgbwColor color) {
  for (int i = first; i < last; i++) {
    strip.SetPixelColor(i, color);
  }
}
/**
   Fill a solid strip from beginning to @code{last} (non-inclusive).
*/
void fillSolid(int last, RgbwColor color) {
  for (int i = 0; i < last; i++) {
    strip.SetPixelColor(i, color);
  }
}
/**
   In case no range is given, fill the whole strip with the given color.
*/
void fillSolid(RgbwColor color) {
  for (int i = 0; i < NUM_LEDs; i++) {
    strip.SetPixelColor(i, color);
  }
}


/**
   Add a color block from @code{start_point} to @code{end_point}.
   These parameters are float values.
*/
void fillFloat(float start_point, float end_point, RgbwColor color) {
  fillFloat(start_point, end_point, color, true);
}
/**
   Fill a color block from @code{start_point} to @code{end_point}.
   These parameters are float values.
   The parameter @code{add} determines whether colors are added or overridden.
*/
void fillFloat(float start_point, float end_point, RgbwColor color, bool add) {

  // first segment
  if (end_point - start_point < start_point - (int) start_point) {
    // only on one pixel

    uint8_t dim = 255 * (1 - (end_point - start_point));
    RgbwColor res_color = color.Dim(dim);
    addPixelColor((int) start_point, res_color);
    return;
  } else {
    // spans multiple pixels

    uint8_t dim = 255 * (1 - start_point - (int) start_point);
    RgbwColor res_color = color.Dim(dim);
    addPixelColor((int) start_point, res_color);
  }

  // middle segment
  if (add) {
    addSolid(ceil(start_point), floor(end_point), color);
  } else {
    fillSolid(ceil(start_point), floor(end_point), color);
  }

  // last segment
  uint8_t dim = 255 * (end_point - (int) end_point);
  RgbwColor res_color = color.Dim(dim);
  if (add) {
    addPixelColor((int) end_point, res_color);
  } else {
    strip.SetPixelColor((int) end_point, res_color);
  }

}


/**
    Add a color block from @code{start_position} to @code{end_position}
    to already existing colors on led strip.
*/
void addSolid(uint8_t start_position, uint8_t end_position, RgbwColor color) {
  for (int i = 0; i < end_position - start_position; i++) {
    addPixelColor(start_position + i, color);
  }
}


/**
   change any light on strip to a white sparkle.
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


/**
   Add a color to a pixel instead of overwriting it.
*/
void addPixelColor(int pixel, RgbwColor add) {
  RgbwColor current = strip.GetPixelColor(pixel);
  RgbwColor result = RgbwColor(
                       constrain(current.R + add.R, 0, 255),
                       constrain(current.G + add.G, 0, 255),
                       constrain(current.B + add.B, 0, 255),
                       constrain(current.W + add.W, 0, 255)
                     );
  strip.SetPixelColor(pixel, result);
}


/**
   Fade all lights to black by @code{amount} steps.
   @code{amount} € [0,255]
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

/**
   Fade all lights to black by a random amount each.
*/
void fadeToBlackRandom() {
  fadeToBlackRandom(0, 128);
}
/**

   Fade all lights to black by a random amount each.
   Bounds are given by @code{lower} and @code{upper}.
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

/**
   Multiply each led color by some factor.
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


/**
   Clear all leds on the strip.
*/
void clear_strip() {
  for (uint8_t i = 0; i < NUM_LEDs; i++) {
    strip.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
  }
}
