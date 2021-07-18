
/* ------------------------------------------------------------------------- */
/* file imports */
#pragma once

#include <NeoPixelBus.h>      // LED library

#include <EEPROM.h>           // EEPROM communication

#include <vector>             // vectors

#include "main_vars.h"           // master variables
#include "led_functions.h"    // basic led functions
#include "utils.h"            // 
#include "animations.h"
#include "pixels.h"

using namespace std;



/* ------------------------------------------------------------------------- */
// input handling

float BRIGHTNESS = 0, MOD = 0;
bool LEFT_BUTTON, RIGHT_BUTTON;

const float INPUT_SMOOTHING = 0.85; // amount of smoothing: [0, 1]



/* ------------------------------------------------------------------------- */
/* led settings */

float MAX_MILLIAMPS = INFINITY;     // max amount of milliamps to draw
NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(NUM_LEDs, LED_PIN);



/* ------------------------------------------------------------------------- */
/* animation handling */

vector<Animation*> animations;      // all animations

uint8_t ACTIVE_ANIMATION = 0;       // currently active animation
uint8_t ANIMATION_COUNT;            // amount of animations
int8_t ANIMATION_TRANSITION = 0;    // direction change: [-1, 0, 1]



/* ========================================================================= */
// main functions

void setup()
{
  /* util setup */
  Serial.begin(115200);
  randomSeed(analogRead(0));

  /* I/O setup*/
  pinMode(POTI_B_PIN, INPUT);
  pinMode(POTI_M_PIN, INPUT);
  pinMode(BTN_L_PIN, INPUT_PULLUP);
  pinMode(BTN_R_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  EEPROM.begin(EEPROM_SIZE);

  /* set lamp parameters */
  init_lamp();

  /* set animations */
  animations.push_back(new PlainWhite());
  animations.push_back(new HueLight());
  animations.push_back(new DiagBars());
  animations.push_back(new EdgeColors());
  // animations.push_back(new MarchEdges());

  ANIMATION_COUNT = animations.size();
  ACTIVE_ANIMATION = EEPROM.read(0) % ANIMATION_COUNT;

  /* led setup */
  strip.Begin();
  strip.Show();
}


void loop() {
  /* timing variables */
  static unsigned long frame_start;
  static unsigned long frame_time;

  // measure frame start time
  frame_start = millis();
  
  update_inputs();
  
  // draw current animation
  animations[ACTIVE_ANIMATION]->update();
  animations[ACTIVE_ANIMATION]->draw();

  // (possibly) fade to other animation
  if (ANIMATION_TRANSITION == 0) {
    // only update scene transitions when none are happening
    update_animation_params();
  } else {
    // oddly long calculation to compensate negative modulo
    uint8_t idx = (ACTIVE_ANIMATION + ANIMATION_TRANSITION + ANIMATION_COUNT) % ANIMATION_COUNT;
    Serial.printf("Switching animation %u -> %u\n", ACTIVE_ANIMATION, idx);
    animation_transition(animations[idx]);
  }

  // show frame
  show();

  // calculate frame delay
  frame_time = millis() - frame_start;
  if(frame_time < FRAME_DELAY) {
    delay(FRAME_DELAY - frame_time);
  }
}
