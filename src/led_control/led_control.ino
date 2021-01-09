
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
#define POTI_M_PIN 35
#define BTN_L_PIN 32
#define BTN_R_PIN 33
#define LED_PIN 22

vector<Input*> inputs;

//----------------------------------------------------------------------
// led settings

const int NUM_LEDs = 46;
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
  
  vector<tuple<int, int>> input_values = {
    make_tuple(POTI, POTI_B_PIN),
    make_tuple(POTI, POTI_M_PIN),
    make_tuple(BUTTON, BTN_L_PIN),
    make_tuple(BUTTON, BTN_R_PIN)
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

  DrawBallBounce();
  DrawBallBounce2();

  show();
  delay(20);
}
