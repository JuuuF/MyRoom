
//----------------------------------------------------------------------
// imports
#pragma once

#include <NeoPixelBus.h>
//#include <NeoPixelBrightnessBus.h>
//#include <NeoPixelSegmentBus.h>
//#include <NeoPixelAnimator.h>

#include <EEPROM.h>

#include <vector>
// #include <tuple>

#include "macros.h"
#include "led_functions.h"
#include "utils.h"
#include "animations.h"
#include "pixels.h"

using namespace std;



//----------------------------------------------------------------------
// I/O


float BRIGHTNESS = 0, MOD = 0;
bool LEFT_BUTTON, RIGHT_BUTTON;

const float INPUT_SMOOTHING = 0.85;


// vector<Input*> inputs;


//----------------------------------------------------------------------
// led settings

// #define NUM_LEDs 392;
float MAX_MILLIAMPS = INFINITY;
NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(NUM_LEDs, LED_PIN);

extern Pixel lamp[];
extern int lamp_x, lamp_y;


//----------------------------------------------------------------------
// utils


vector<Animation*> animations;

uint8_t ACTIVE_ANIMATION = 0;       // currently active animation // TODO: export into flash
uint8_t ANIMATION_COUNT;            // amount of animations
int8_t ANIMATION_TRANSITION = 0;    // direction change, values [-1, 0, 1]


//======================================================================
// main functions

void setup()
{
  Serial.begin(115200);
  
  randomSeed(analogRead(0));

  // I/O declaration

  EEPROM.begin(EEPROM_SIZE);

  // vector<tuple<int, int, byte>> input_values = {
  //   make_tuple(POTI, POTI_B_PIN, BRIGHTNESS),
  //   make_tuple(POTI, POTI_M_PIN, MOD),
  //   make_tuple(BUTTON, BTN_L_PIN, LEFT_BUTTON),
  //   make_tuple(BUTTON, BTN_R_PIN, RIGHT_BUTTON)
  // };
  // inputs = set_inputs(input_values);
  pinMode(POTI_B_PIN, INPUT);
  pinMode(POTI_M_PIN, INPUT);
  pinMode(BTN_L_PIN, INPUT_PULLUP);
  pinMode(BTN_R_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);

  // initialize LED strip
  //setMaxMilliamps(900);

  // set animations
  animations.push_back(new PlainWhite());
  animations.push_back(new BlueLight());
  animations.push_back(new DiagBars());

  ANIMATION_COUNT = animations.size();
  ACTIVE_ANIMATION = EEPROM.read(0);

  strip.Begin();
  strip.Show();
  Serial.println("END SETUP");

}


void loop() {
  static unsigned long frame_start = millis();
  static unsigned long frame_time;
  
  update_inputs();

  /*
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
  */
  
  




  
  // draw current animation
  animations[ACTIVE_ANIMATION]->update();
  animations[ACTIVE_ANIMATION]->draw();

  // (possibly) fade to other animation
  if (ANIMATION_TRANSITION == 0) {
    // only update scene transitions when none are happening
    update_animation_params();
  } else {
    animation_transition(
      animations[(ACTIVE_ANIMATION + ANIMATION_TRANSITION + ANIMATION_COUNT) % ANIMATION_COUNT]
    );
  }

  // stop. wait a minute.
  show();

  frame_time = millis() - frame_start;
  if(frame_time < 25) {
    delay(25 - frame_time);
  }
  frame_start = millis();
  // Serial.printf("frame time: %d\ndelay: %d\n\n", frame_time, 25-frame_time);


}
