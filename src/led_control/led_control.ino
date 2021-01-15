
//----------------------------------------------------------------------
// imports

#include <NeoPixelBus.h>
//#include <NeoPixelBrightnessBus.h>
//#include <NeoPixelSegmentBus.h>
//#include <NeoPixelAnimator.h>

#include <vector>
#include <tuple>

#include "led_functions.h"
#include "utils.h"
#include "animations.h"

using namespace std;



//----------------------------------------------------------------------
// I/O


#define POTI_B_PIN 34
const byte BRIGHTNESS = 0;

#define POTI_M_PIN 35
const byte MOD = 1;

#define BTN_L_PIN 32
const byte LEFT_BUTTON = 2;

#define BTN_R_PIN 33
const byte RIGHT_BUTTON = 3;

#define LED_PIN 22

vector<Input*> inputs;

//----------------------------------------------------------------------
// led settings

const int NUM_LEDs = 57;
float MAX_MILLIAMPS = INFINITY;

NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(NUM_LEDs, LED_PIN);



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
  setMaxMilliamps(900);

  strip.Begin();
  strip.Show();

}

void loop() {

  update_inputs();
  set_scene();

  static BouncingBall* b1 = new BouncingBall();
  
  static BouncingBall* b2 = new BouncingBall();
  b2->set_dampening(0.06);
  b2->set_frequency(0.02);
  b2->set_time_max(30);
  b2->set_reversed(true);

  fadeToBlackBy(get_poti(MOD) * 16);

  b1->update();
  b1->draw();
  b2->update();
  b2->draw();

  /*
    DrawBallBounce();
    DrawBallBounce2();
  */

  show();
  delay(20);
}
