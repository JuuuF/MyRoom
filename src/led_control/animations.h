
//----------------------------------------------------------------
// animations

#pragma once
#include "led_functions.h"
#include "utils.h"

extern const int NUM_LEDs;

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

void DrawBallBounce() {

  static float pos;
  static const float e = 2.71828;
  static const float k = 0.05;
  static const float w = 0.01;
  static const int h = NUM_LEDs;
  static float t = random(50);
  static int dir = 1;
  static const float step = 0.1;

  static const float size = 2.0;

  static float color_picker = random(360);
  static const float color_delta = 0.7;

  // get position
  pos = abs(h * exp(-k * t) * cos(w * pow(t, 2)));

  // draw
  addFloat(pos, pos + size, Hsvw2Rgbw(color_picker, 1, 0.5, 0)); //poti_b_val, 0), true);

  // update params
  t += step * dir;
  color_picker += color_delta;
  if (color_picker > 360) {
    color_picker -= 360;
  }

  // reverse animation
  if (t > 50 || t < 0) {
    dir *= -1;
  }
}

void DrawBallBounce2() {

  static float pos;
  static const float e = 2.71828;
  static const float k = 0.06;
  static const float w = 0.02;
  static const int h = NUM_LEDs;
  static float t = random(40);
  static int dir = -1;
  static const float step = 0.1;

  static const float size = 2.0;

  static float color_picker = random(360);
  static const float color_delta = 1.0;

  // get position
  pos = h - abs(h * exp(-k * t) * cos(w * pow(t, 2)));

  // draw
  addFloat(pos, pos + size, Hsvw2Rgbw(color_picker, 1, 0.5,0));// poti_b_val, 0), true);

  // update params
  t += step * dir;
  color_picker += color_delta;
  if (color_picker > 360) {
    color_picker -= 360;
  }

  // reverse animation
  if (t > 40 || t < 0) {
    dir *= -1;
  }
}
