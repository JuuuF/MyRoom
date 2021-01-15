
//----------------------------------------------------------------
// animations

#pragma once
#include "led_functions.h"
#include "utils.h"

using namespace std;

extern const int NUM_LEDs;

extern const byte BRIGHTNESS, MOD, LEFT_BUTTON, RIGHT_BUTTON;

void DrawPingPong() {
  static float block_len = 3.0;
  static int dir = 1;
  static float start_pos = 0.0, end_pos = NUM_LEDs;
  static float pos = 1.0;
  static float color_picker = 0.0;
  static float color_delta = 5;

  float speed = 4; //poti_m_val * 4;

  // draw block
  fadeToBlackRandom();//12 * poti_m_val, 32 * poti_m_val);
  // randomSparkles();
  RgbwColor color = Hsvw2Rgbw(color_picker, 1.0, 0.5, 0);//poti_b_val, 0);
  addFloat(pos, pos + block_len, color);


  // end check
  if (
    (pos + block_len > end_pos   && dir ==  1) ||   //
    (pos             < start_pos && dir == -1)) {   //
    dir *= -1;
  }

  // update position
  pos += speed * dir;

  color_picker += color_delta;
  if (color_picker > 360)
    color_picker -= 360;

}

/** ====================================================================
  BouncingBall

  Not-so-realistic bouncing ball effect that changes in color.
  Bouncing behavour based on equation:
    y = abs( _h * e^( _d * x ) * cos( _w * x^2 ))
*/
class BouncingBall {
  private:
    // changing variables
    float _pos;                   // current position
    float _t;                     // current time
    int _dir = 1;                 // current direction
    float _color_hue;             // current hue color
    float _h;                     // height from _start_pos to _end_pos

    // "constant" variables which are technically not constant
    // because C++ does not allow for arbitrary optional params to be set. thanks for that.
    float _d;                     // dampening factor
    float _f;                     // frequency modifier
    float _start_point;           // start point
    float _end_point;             // end point
    float _step;                  // step size between updates
    float _size;                  // ball size
    float _color_delta;           // color change per update
    float _time_max;              // time until animation reverse
    bool _reversed;               // direction of bounce

  public:
    /** ----------------------------------------------------------------
      Constructor
    */
    BouncingBall()
    {
      // initialize default values
      // C++ does not let you choose an optional parameter to set, so we have to go this way
      _d = 0.05;
      _f = 0.01;
      _start_point = 0;
      _end_point = NUM_LEDs;
      _step = 0.1;
      _size = 2.0;
      _color_delta  = 0.7;
      _time_max = 40;
      _reversed = false;
      _pos = random(_start_point, _end_point);
      _t = random(_time_max);
      _dir = random(2) == 0 ? -1 : 1;
      _color_hue = random(360);
      _h = _end_point - _start_point;
    }

    // -----------------------------------------------------------------
    // update / draw

    /**
       update

       Advance the current state by one step.
    */
    void update() {

      // get position
      _pos = abs(_h * exp(-_d * _t) * cos(_f * (_t * _t)));
      if (_reversed)
        _pos = _end_point - _pos;

      // update params
      _t += _step * _dir;
      _color_hue += _color_delta;
      if (_color_hue > 360) {
        _color_hue -= 360;
      }

      // reverse animation
      if (_t > _time_max || _t < 0) {
        _dir *= -1;
      }

    }

    /**
      draw

      Draw the current state to the led strip.
    */
    void draw() {
      float brightness = get_poti(BRIGHTNESS);
      addFloat(_pos, _pos + _size, Hsvw2Rgbw(_color_hue, 1, brightness, 0));
    }


    // -----------------------------------------------------------------
    // setters for "constant" variables.
    // These would be passed to the constructor, but C++ does not let you
    // set arbitrary optional parameters. So this is the least bad

    void set_dampening(float d) {
      _d = d;
    }

    void set_frequency(float f) {
      _f = f;
    }

    void set_start_point(float pos) {
      _start_point = pos;
    }

    void set_end_point(float pos) {
      _end_point = pos;
    }

    void set_step(float step) {
      _step = step;
    }

    void set_size(float size) {
      _size = size;
    }

    void set_color_delta(float delta) {
      _color_delta = delta;
    }

    void set_time_max(float time_max) {
      _time_max = time_max;
    }

    void set_reversed(bool reversed) {
      _reversed = reversed;
    }

};



/** ====================================================================

*/
class ColorWheel1D {
  private:
    float _r;               // raduis of wheel
    float _m;               // mid point of wheel

    float _rot;                   // current rotation
    float _speed;                 // max speed
    float _t;                     // time
    float _dt;                    // time delta

  public:
    /* constructor */
    ColorWheel1D(float r = NUM_LEDs / 2.0, float m = NUM_LEDs / 2.0, float speed = 10.0)
      : _r(r), _m(m), _speed(speed) {
      _rot = random(360);
      _t = random(360);
      _dt = random(200) / 20.0;
    }


    void update() {
      float d = _dt; // * get_poti(MOD);
      _t = (_t + d < 360) ? _t + d - 360 : _t + d;
      _m = (sin(_t / 180 * PI) + 1) * NUM_LEDs / 2;
      _rot = fmod(_rot + _speed, 360);
    }

    void draw() {
      // fadeToBlackBy(4);
      for (int i = 0; i < NUM_LEDs; i++) {

        // outside wheel
        if (abs(i - _m) > _r)
          continue;

        float h = _rot;//cos(_pos/180 * PI)*360;

        if (i - _m < 0) {
          h = (h + 180) > 360 ? h - 180 : h + 180;
        }
        float s = abs(i - _m) / _r;
        s *= 1.1;   // make the ends c o l o r f u l
        float v = get_poti(BRIGHTNESS) / 2;
        RgbwColor col = Hsvw2Rgbw(h, s, v, 0);
        addPixelColor(i, col);
        //strip.SetPixelColor(i, col);
      }
    }

};
