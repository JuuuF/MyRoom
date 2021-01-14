
#pragma once
#include "led_functions.h"

#include <vector>
#include <tuple>

extern const int NUM_LEDs;
extern float MAX_MILLIAMPS;
extern NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip;

using namespace std;

//======================================================================
// Input handling



/**---------------------------------------------------------------------
  InputTypes

  Enumeration for different input types.
*/
enum InputTypes { POTI, BUTTON };


/**---------------------------------------------------------------------
  Input

  Generic class for inputs. Input pin needs to be declared as input for
  controller beforehand.

  Params:
    int PIN                     hardware pin of input.
    byte identifier             identifier / name for the input.
*/
class Input {
  public:
    const int PIN;              // input pin
    const byte ID;              // identifier for input

    /* constructor */
    Input(int pin, byte identifier)
      : PIN(pin),
        ID(identifier) {};

    /* update input state */
    virtual void update() = 0;
};

/**---------------------------------------------------------------------
  Button

  Button input. Keeps track of button state as well as rising and falling
  edge state.
*/
class Button : public Input {
  private:
    bool _current, _rising, _falling;  // button states

  public:
    /* constructor */
    Button(int pin, byte identifier) : Input(pin, identifier) {
      // initialize inputs
      update();
    }


    /*
      update

      Update the button's states reguarding the current input.

      Sets:
        _current                true if pressed
        _rising                 true if rising edge
        _falling                true if falling edge
    */
    void update() {
      // get current state
      bool last = _current;
      _current = digitalRead(PIN) == HIGH;

      // rising edge
      _rising = _current && !last;

      // falling edge
      _falling = !_current && last;
    }


    /* getter functions */
    bool is_pressed() {
      return _current;
    }

    bool is_rising() {
      return _rising;
    }

    bool is_falling() {
      return _falling;
    }

};

/**---------------------------------------------------------------------
  Poti

  Potentiometer input. Keeps track of an analog input on a specified pin.
  Input will be normalized to interval [0,1].
*/
class Poti : public Input {
  private:
    float value;                    // current value
    float input_resolution;         // input resolution in bit
    float noise_filtering = 0.005;  // filtering factor

  public:
    /* constructor */
    Poti(int pin, byte identifier, int res = 4096) : Input(pin, identifier) {
      input_resolution = res;
      update();
    };

    /**
      update

      Update poti value.

      Sets:
        value                       current reading of poti
    */
    void update() {

      float new_value = analogRead(PIN) / 4096.0;
      if (abs(new_value - value) > noise_filtering) {
        value = new_value;
      }
    }

    /* getter functions */
    float get_value() {
      return value;
    }

    /* setter functions */
    void set_noise_filtering(float val) {
      noise_filtering = val;
    }

};



/**---------------------------------------------------------------------
  update_inputs

  Update all inputs.
*/
extern std::vector<Input*> inputs;

void update_inputs() {
  for (int i = 0; i < inputs.size(); i++) {
    inputs[i]->update();
  }
}



/**---------------------------------------------------------------------
  set_inputs

  Set up the inputs for the project. Inputs are given as a map.
  The first value is the type of input and the second value is the
  input pin.
  Input type is expected to be from enumeration InputType.

  Params:
    vector<tuple<int, int, byte>> inputs:
        inputs as vector of tuples.
        - first int:    type of input (see InputType)
        - second int:   input pin
        - byte:         input identifier / name
*/
vector<Input*> set_inputs(vector<tuple<int, int, byte>> inputs) {

  vector<Input*> res;

  for (int i = 0; i < inputs.size(); i++) {

    if (get<0>(inputs[i]) == POTI) {
      // poti input
      res.push_back(new Poti(get<1>(inputs[i]), get<2>(inputs[i])));
      pinMode(get<1>(inputs[i]), INPUT);

    } else if (get<0>(inputs[i]) == BUTTON) {
      // button input
      res.push_back(new Button(get<1>(inputs[i]), get<2>(inputs[i])));
      pinMode(get<1>(inputs[i]), INPUT_PULLUP);
    } else {
      // unknown input
      printf("WARNING: input type %i is not specified. Skipping input %i.", get<0>(inputs[i]), i);
    }
  }

  return res;
}

// TODO: get_inputs(int input_idx) function, input_idx from InputIndices<

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


//======================================================================
// other functions



/**---------------------------------------------------------------------
  set_scene

  Set the current cene number depending on button inputs.

  Sets:
    int SCENE                   currently selected scene.
*/
void set_scene() {

  /*
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
  */
}
