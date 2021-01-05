
//----------------------------------------------------------------
// util functions

/**
   Read in all inputs and set corresponding variables.
*/
void read_inputs() {

  // potis

  float noise_filtering = 0.005;

  float new_poti_b_val = analogRead(POTI_B_PIN) / 4096.0;
  if (abs(new_poti_b_val - poti_b_val) > noise_filtering) {
    poti_b_val = new_poti_b_val;
  }
  float new_poti_m_val = analogRead(POTI_M_PIN) / 4096.0;
  if (abs(new_poti_m_val - poti_m_val) > noise_filtering) {
    poti_m_val = new_poti_m_val;
  }

  // ---------------
  // buttons

  static bool in_l, in_r;
  bool last_in_l = in_l;
  bool last_in_r = in_r;

  in_l = digitalRead(BTN_L_PIN) == HIGH;
  in_r = digitalRead(BTN_R_PIN) == HIGH;

  // detect falling edge
  btn_l = !in_l && last_in_l;
  btn_r = !in_r && last_in_r;

}


/**
   Set the scene number depending on button inputs.
*/
void set_scene() {


  if (btn_l) {
    // left button
    scene--;
    if (scene < 0)
      scene = scene_count - 1;
  }
  if (btn_r) {
    // right button
    scene++;
    if (scene >= scene_count)
      scene = 0;
  }
}


/**
   HSVW to RBGW color conversion.

   source: https://www.codespeedy.com/hsv-to-rgb-in-cpp/
*/
RgbwColor Hsvw2Rgbw(float H, float S, float V, float W) {

  if (H > 360 || H < 0 || S > 1 || S < 0 || V > 1 || V < 0) {
    return RgbwColor(0);
  }

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
  int R = (r + m) * 255;
  int G = (g + m) * 255;
  int B = (b + m) * 255;

  return RgbwColor(R, G, B, W);
}


/**
   Get the maximum color value of a given color.
*/
int maxColorVal(RgbwColor color) {
  return max(max(color.R, color.G), max(color.B, color.W));
}

/**
   Set maximum milliamps to output.
*/
void setMaxMilliAmps(float val) {
  MAX_MILLIAMPS = val;
}

/**
   Draw leds to strip keeping MAX_MILLIAMPS in mind.
*/
void show() {
  int draw = calculateMilliAmps();
  if (draw > MAX_MILLIAMPS) {
    // darken all leds
    float fac = (float) MAX_MILLIAMPS / draw;
    multByFactor(fac * 0.95); // just to be sure
  }

  if (calculateMilliAmps() > MAX_MILLIAMPS) {
    RgbwColor origin = strip.GetPixelColor(0);
    // warning blinks
    for (int i = 0; i < 3; i++) {
      strip.SetPixelColor(0, RgbwColor(255, 0, 0, 0));
      strip.Show();
      delay(10);
      strip.SetPixelColor(0, RgbwColor(0));
      strip.Show();
      delay(10);
    }
    strip.SetPixelColor(0, origin);
  }

  strip.Show();
}

/**
   Calculate the milliamps used to draw the current strip state.
*/
int calculateMilliAmps() {
  int total = 0;
  for (int i = 0; i < NUM_LEDs; i++) {
    RgbwColor col = strip.GetPixelColor(i);
    total += col.R / 255.0 * 20; // max. 20 mAh per led
    total += col.G / 255.0 * 20;
    total += col.B / 255.0 * 20;
    total += col.W / 255.0 * 20;
  }
  return total;
}

/**
   Delay until frame is over

  void delay_fps(long start_time) {
  long now = millis();

  if (now - start_time < MSPS) {
    delay(MSPS - (now - start_time));
  }
  }
*/
