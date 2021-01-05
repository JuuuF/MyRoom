

//======================================================================
// led getter functions



/**---------------------------------------------------------------------
  maxColorVal
  
  Get the maximum color value of a given color.

  Params:
    RgbwColor color             color.

  Returns:
    int                         max of all color values.
*/

int maxColorVal(RgbwColor color) {
  return max(max(color.R, color.G), max(color.B, color.W));
}



/**---------------------------------------------------------------------
  calculateMilliAmps
  
  Calculate the milliamps used to display the current strip state.

  Returns:
    int                         current drawn in milliamps.
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



//======================================================================
// led setter functions



/**---------------------------------------------------------------------
  setMaxMilliamps
  
  Set maximum milliamps to draw.

  Params:
    float val                   max current to draw in milliamps.

  Sets:
    float MAX_MILLIAMPS         max milliamps to draw.
*/
void setMaxMilliamps(float val) {
  MAX_MILLIAMPS = val;
}



//======================================================================
// other led functions



/**---------------------------------------------------------------------
  Hsvw2Rgbw

  
  Convert HSVW color to RBGW color.

  Source: https://www.codespeedy.com/hsv-to-rgb-in-cpp/

  Params:
    float H: [0,360]            hue.
    float S: [0,1]              saturation.
    float V: [0,1]              value.
    float W: [0,255]            white component.

  Returns:
    RgbwColor                   black     , if invalid inputs
                                rgb color , else
*/
RgbwColor Hsvw2Rgbw(float H, float S, float V, float W) {

  if (H > 360 || H < 0 || S > 1 || S < 0 || V > 1 || V < 0 || W > 255 || W < 0) {
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

  return RgbwColor(R, G, B, (int) W);
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
// I/O functions



/**---------------------------------------------------------------------
  read_inputs
  
  Read in all inputs and set corresponding variables.

  Sets:
    float poti_b_val            value of brightness poti: [0,1].
    float poti_m_val            value of modulation poti: [0,1].
    bool btn_l                  true if falling edge on left button.
    bool btn_r                  true if falling edge on right button.
*/
void read_inputs() {

  // ---------------
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



//======================================================================
// other functions



/**---------------------------------------------------------------------
  set_scene

  Set the current cene number depending on button inputs.

  Sets:
    int SCENE                   currently selected scene.
*/
void set_scene() {


  if (btn_l) {
    // left button
    SCENE--;
    if (SCENE < 0)
      SCENE = SCENE_COUNT - 1;
  }
  if (btn_r) {
    // right button
    SCENE++;
    if (SCENE >= SCENE_COUNT)
      SCENE = 0;
  }
}
