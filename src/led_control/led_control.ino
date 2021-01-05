
//----------------------------------------------------------------------
// imports

#include <NeoPixelBus.h>
//#include <NeoPixelBrightnessBus.h>
//#include <NeoPixelSegmentBus.h>
//#include <NeoPixelAnimator.h>

using namespace std;



//----------------------------------------------------------------------
// I/O

#define POTI_B_PIN 34
#define POTI_M_PIN 35
#define BTN_L_PIN 32
#define BTN_R_PIN 33
#define LED_PIN 22

/* poti variables, normalized to [0,1] */
float poti_b_val, poti_m_val;
/* button states, true on falling edge */
bool btn_l, btn_r;



//----------------------------------------------------------------------
// led settings

#define NUM_LEDs 46
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
  // I/O declaration
  pinMode(POTI_B_PIN, INPUT);
  pinMode(POTI_M_PIN, INPUT);
  pinMode(BTN_L_PIN, INPUT_PULLUP);
  pinMode(BTN_R_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  randomSeed(analogRead(0));

  // initialize LED strip
  setMaxMilliamps(900);

  strip.Begin();
  strip.Show();

}

void loop() {
  read_inputs();
  set_scene();

  fadeToBlackBy(poti_m_val * 16);
  DrawBallBounce();
  DrawBallBounce2();

  show();
  delay(20);
}
