
// imports

#include <NeoPixelBus.h>
//#include <NeoPixelBrightnessBus.h>
//#include <NeoPixelSegmentBus.h>
//#include <NeoPixelAnimator.h>



// I/O

#define POTI_B_PIN 34
#define POTI_M_PIN 35
#define BTN_L_PIN 32
#define BTN_R_PIN 33
#define LED_PIN 22

// input variables
float poti_b_val, poti_m_val;         // normalized to [0,1]
bool btn_l, btn_r;                    // true if button released



// led settings

#define NUM_LEDs 46
float MAX_MILLIAMPS = INFINITY;
NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(NUM_LEDs, LED_PIN);



// utils

int scene = 0;
int scene_count = 4;



//----------------------------------------------------------------
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
  setMaxMilliAmps(900);
  
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
