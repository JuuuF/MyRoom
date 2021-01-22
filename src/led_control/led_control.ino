
//----------------------------------------------------------------------
// imports
#pragma once

#include <NeoPixelBus.h>
//#include <NeoPixelBrightnessBus.h>
//#include <NeoPixelSegmentBus.h>
//#include <NeoPixelAnimator.h>

#include <vector>
#include <tuple>

#include "led_functions.h"
#include "utils.h"
#include "animations.h"
#include "pixels.h"
#include "scenes.h"

using namespace std;



//----------------------------------------------------------------------
// I/O


#define POTI_B_PIN 34
const byte BRIGHTNESS = 0;

#define POTI_M_PIN 35
const byte MOD = 1;

#define BTN_L_PIN 2
const byte LEFT_BUTTON = 2;

#define BTN_R_PIN 15
const byte RIGHT_BUTTON = 3;

#define LED_PIN 22

vector<Input*> inputs;

//----------------------------------------------------------------------
// led settings

const int NUM_LEDs = 392;
float MAX_MILLIAMPS = INFINITY;
NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(NUM_LEDs, LED_PIN);

extern Pixel lamp[];
extern int lamp_x, lamp_y;


//----------------------------------------------------------------------
// utils

int SCENE = 0;
int SCENE_COUNT = 4;



//======================================================================
// main functions

void setup()
{
  randomSeed(analogRead(0));

  // I/O declaration

  vector<tuple<int, int, byte>> input_values = {
    make_tuple(POTI, POTI_B_PIN, BRIGHTNESS),
    make_tuple(POTI, POTI_M_PIN, MOD),
    make_tuple(BUTTON, BTN_L_PIN, LEFT_BUTTON),
    make_tuple(BUTTON, BTN_R_PIN, RIGHT_BUTTON)
  };
  inputs = set_inputs(input_values);

  pinMode(LED_PIN, OUTPUT);

  // initialize LED strip
  //setMaxMilliamps(900);

  strip.Begin();
  strip.Show();

}


void loop() {

  update_inputs();

  int scene_change = set_scene();

  if (scene_change != 0) {
    // scene transition using scene_change
    clear_strip();
  }

  static BouncingBall b1;
  b1.set_size(20);
  static BouncingBall b2;
  b2.set_dampening(0.07);
  b2.set_size(20);
  b2.set_time_max(40);
  b2.set_reversed(true);

  if (SCENE == 0) {
    fadeToBlackBy(16 * get_poti(MOD));
    b1.update();
    b2.update();
    b1.draw();
    b2.draw();
    delay(20);

  } else if (SCENE == 1) {
    march_edges();

  } else if (SCENE == 2) {
    fadeToBlackBy(16 * get_poti(MOD));
    diag_bars();
    delay(20);

  } else if (SCENE == 3) {
    fadeToBlackBy(16 * get_poti(MOD));
    sine_wave();
    delay(10);

  }

  show();

}
