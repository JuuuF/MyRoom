
/** ===========================================================================
  animations.h

  This file handles all animations.
  Animations implement the (pure) virtual functions of the Animation class.
  This includes an update function, a draw function and optionally a getState
  function, which writes the current animation state to a given buffer.
*/

#pragma once

#include "led_functions.h"
#include "utils.h"
#include "pixels.h"

using namespace std;

/* lamp variables */
extern Pixel lamp[];
extern uint16_t lamp_x, lamp_y;

/* I/O variables */
extern float BRIGHTNESS, MOD;
extern bool LEFT_BUTTON, RIGHT_BUTTON;

/* animation variables */
extern uint8_t ACTIVE_ANIMATION;
extern uint8_t ANIMATION_COUNT;
extern int8_t ANIMATION_TRANSITION;


/* ========================================================================= */

/** -----------------------------------------------------------------
  Animation

  This interface class functions as a building block for animations.
  It provides pure virtual functions to update and draw the animation that have
  to be implemented for each animaiton accordingly. Additionally it provides
  an optional function to write the current animation state to a buffer.
 */
class Animation
{
  public:
    /* update inner params */
    virtual void update() = 0;
    /* draw state to led strip */
    virtual void draw() = 0;
    /* get the current state as RgbwColor array of size NUM_LEDs */
    virtual void getState(RgbwColor* buffer) { }
};



/* ========================================================================= */
/* animations */

/** -----------------------------------------------------------------
  PlainWhite

  Plain white scene.
  Can be modified to change brightness and color temperature.
*/
class PlainWhite : public Animation
{
  private:
    float _ratio = 0.9;             // filter ratio
    float _neutral_deadband = 0.1;  // neutral deadband in percent
    uint8_t _w[4] = {255, 237, 0,   0};    // warm color values
    uint8_t _n[4] = {0,   0,   0,   255};  // neutral color values
    uint8_t _c[4] = {64,  100, 239, 0};    // cold color values

    /* Get color based on current parameters */
    RgbwColor _get_color() {
      if (MOD < 0.5 - _neutral_deadband) {
        // cold
        float ratio = MOD / (0.5 - _neutral_deadband);
        return RgbwColor(
          _c[0] * (1 - ratio) + _n[0] * ratio,
          _c[1] * (1 - ratio) + _n[1] * ratio,
          _c[2] * (1 - ratio) + _n[2] * ratio,
          _c[3] * (1 - ratio) + _n[3] * ratio
        ).Dim(BRIGHTNESS * 255);
      } else if (MOD > 0.5 + _neutral_deadband) {
        //warm
        float ratio = (1 - MOD) / (0.5 - _neutral_deadband);
        return RgbwColor(
          _w[0] * (1 - ratio) + _n[0] * ratio,
          _w[1] * (1 - ratio) + _n[1] * ratio,
          _w[2] * (1 - ratio) + _n[2] * ratio,
          _w[3] * (1 - ratio) + _n[3] * ratio
        ).Dim(BRIGHTNESS * 255);
      } else {
        // neutral
        return RgbwColor(
          _n[0], 
          _n[1], 
          _n[2], 
          _n[3]
        ).Dim(BRIGHTNESS * 255);
      }
    }
  
  public:
    PlainWhite()
    {
      update();
      draw();
    };

    /* update current state */
    void update() override {}

    /* draw current state to led strip */
    void draw() override
    {
      setSolid(0, NUM_LEDs, _get_color());
    }

    void getState(RgbwColor* buffer) override
    {
      RgbwColor color = _get_color();
      for (uint16_t i = 0; i < NUM_LEDs; i++) {
        buffer[i] = color;
      }
    }
};



/** -----------------------------------------------------------------
  HueLight

  Visualisation of the HSV color space. Saturation is set to 1.

  BRIGHTNESS controls value
  MOD controls hue
*/
class HueLight : public Animation
{
  public:
    HueLight() {}

    void update() override {}

    void draw() override
    {
      setSolid(Hsvw2Rgbw(MOD * 360, 1, BRIGHTNESS, 0));
    }

    void getState(RgbwColor* buffer) override
    {
      for (int i = 0; i < NUM_LEDs; i++) {
        buffer[i] = Hsvw2Rgbw(MOD * 360, 1, BRIGHTNESS, 0);
      }
    }
};



/** -----------------------------------------------------------------
  DiagBars

  Two diagonal bars of random colors going ham.

  BRIGHTNESS controls brightness
  MOD controls speed
*/
class DiagBars : public Animation
{
  private:
    int _x, _x_dir, _dx;
    int _y, _y_dir, _dy;
    float _hue_x, _sat_x, _hue_y, _sat_y;
    int _speed;

  public:
    DiagBars() :
      _x(random(lamp_x)),
      _x_dir(1),
      _dx(50),
      _y(random(lamp_x)),
      _y_dir(1),
      _dy(50),
      _hue_x(random(360)),
      _sat_x((random(50)/50.0) + 25),
      _hue_y(random(360)),
      _sat_y((random(50)/50.0) + 25),
      _speed(analogRead(POTI_M_PIN) / 4096.0 * 20)
    { }

    void update() override
    {
      _speed = analogRead(POTI_M_PIN) / 4096.0 * 20;

      if (_x >= lamp_x + lamp_y || _x < 0) {
        _x_dir *= -1;
        _x += _speed * _x_dir;
        _hue_x = random(360);
        _sat_x = (random(50)/50.0) + 25;
      }
      _x += _speed * _x_dir;
      _hue_x += 0.5;
      if (_hue_x > 360) {
        _hue_x -= 360;
      }

      if (_y >= lamp_x || _y < -lamp_y) {
        _y_dir *= -1;
        _y += _speed * 1.1 * _y_dir;
        _hue_y = random(360);
        _sat_y = (random(50)/50.0) + 25;
      }
      _y += _speed * 1.1 * _y_dir;
      _hue_y += 0.5;
      if (_hue_y > 360) {
        _hue_y -= 360;
      }
    }

    void draw() override
    {
      fadeToBlackBy(10);
      float dist;
      float wave_fac;
      for (int i = 0; i < NUM_LEDs; i++) {
        // first bar
        dist = abs((lamp[i].x + lamp[i].y) - _x);
        if (dist < _dx) {
          wave_fac = 1 - (dist / _dx);
          addPixel(i, Hsvw2Rgbw(_hue_x, _sat_x, BRIGHTNESS * wave_fac, 0));
        }
        // second bar
        dist = abs((lamp[i].x - lamp[i].y) - _y);
        if (dist < _dy) {
          wave_fac = 1 - (dist / _dy);
          addPixel(i, Hsvw2Rgbw(_hue_y, _sat_y, BRIGHTNESS * wave_fac, 0));
        }
      }
    }

    void getState(RgbwColor* buffer)
    {
      // load current state into buffer
      fadeToBlackBy(10, buffer);
      for (int i = 0; i < NUM_LEDs; i++) {
        if (abs((lamp[i].x + lamp[i].y) - _x) < _dx) {
          RgbwColor col = Hsvw2Rgbw(_hue_x, 1, BRIGHTNESS, 0);
          buffer[i].R = min(buffer[i].R + col.R, 255);
          buffer[i].G = min(buffer[i].G + col.G, 255);
          buffer[i].B = min(buffer[i].B + col.B, 255);
          buffer[i].W = min(buffer[i].W + col.W, 255);
        }
        if (abs((lamp[i].x - lamp[i].y) - _y) < _dy) {
          RgbwColor col = Hsvw2Rgbw(_hue_y, 1, BRIGHTNESS, 0);
          buffer[i].R = min(buffer[i].R + col.R, 255);
          buffer[i].G = min(buffer[i].G + col.G, 255);
          buffer[i].B = min(buffer[i].B + col.B, 255);
          buffer[i].W = min(buffer[i].W + col.W, 255);
        }
      }
    }
};



/* ========================================================================= */
/* animation functions */

/** -----------------------------------------------------------------
  update_animation_params

  Update all global animation parameters. This includes:
    ACTIVE_ANIMATION
    ANIMATION_TRANSITION
*/
void update_animation_params() {
  static bool last_left  = digitalRead(BTN_L_PIN) == HIGH;
  static bool last_right = digitalRead(BTN_R_PIN) == HIGH;

  bool current_left  = digitalRead(BTN_L_PIN) == HIGH;
  bool current_right = digitalRead(BTN_R_PIN) == HIGH;

  if (last_left && !current_left) {
    // left release
    ANIMATION_TRANSITION--;
  } else if (last_right && !current_right) {
    // right release
    ANIMATION_TRANSITION++;
  }
  last_left  = current_left;
  last_right = current_right;
}



/** -----------------------------------------------------------------
  animation_transition

  Fade between animations.
  This method is called when ANIMATION_TRANSITION is not equal to 0. It
  draws the given animaiton alongside a trasnsition bar onto the already
  updated and drawn animation. Afterwards, the ACTIVE_ANIMATION variable
  is updated accordingly.

  The animation to transition to is an overlay to the currently active
  animation.

  Parameters:
    Animation* next_animation           animation to transition to.

*/
void animation_transition(Animation* next_animation) {
  static bool ongoing = false;                    // current state of transition
  static RgbwColor animation_buffer[NUM_LEDs];    // new animation
  static RgbwColor transition_buffer[NUM_LEDs];   // transition animation
  static bool mask[NUM_LEDs];                     // transition mask: false = old, true = new

  /* transition variables */
  static float t_pos = 0;
  static const float t_delta = 42;
  static float t_thickness = 75;

  // first setup
  if (!ongoing) {
    // reset all variables
    ongoing = true;
    for (uint16_t i = 0; i < NUM_LEDs; i++) {
      mask[i] = false;
      transition_buffer[i] = RgbwColor(0);
      animation_buffer[i] = RgbwColor(0);
    }
    // TODO: transition direction depending on ANIMATION_TRANSITION
    t_pos = 0; //ANIMATION_TRANSITION < 0 ? lamp_x : 0;
  }

  // get new animation state
  next_animation->update();
  next_animation->getState(animation_buffer);

  // update params
  t_pos += t_delta;

  Serial.printf("t_pos = %d\n", t_pos);
  // setup transition bar
  fadeToBlackBy(32 * BRIGHTNESS, transition_buffer);

  for (uint16_t i = 0; i < NUM_LEDs; i++) {
    // update transition bar
    // TODO: outsource transition into own class
    if(t_pos - lamp[i].x > 0 && t_pos - lamp[i].x < t_thickness) {
      transition_buffer[i].W = BRIGHTNESS * 255;
      Serial.printf("&d\n", transition_buffer[i].W);
      mask[i] = true;
    }

    // draw new animation
    if (mask[i]) {
      setPixel(i, animation_buffer[i]);
    }

    // add transition bar
    addPixel(i, transition_buffer[i]);
  }

  if(buffer_empty(transition_buffer)) {
    // end of transition
    ACTIVE_ANIMATION += ANIMATION_TRANSITION + ANIMATION_COUNT;
    ACTIVE_ANIMATION %= ANIMATION_COUNT;
    ANIMATION_TRANSITION = 0;
    ongoing = false;

    // store active animation
    EEPROM.write(0, ACTIVE_ANIMATION);
    EEPROM.commit();
  }

}














// void DrawPingPong() {
//   static float block_len = 3.0;
//   static int dir = 1;
//   static float start_pos = 0.0, end_pos = NUM_LEDs;
//   static float pos = 1.0;
//   static float color_picker = 0.0;
//   static float color_delta = 5;

//   float speed = 4; //poti_m_val * 4;

//   // draw block
//   fadeToBlackRandom();//12 * poti_m_val, 32 * poti_m_val);
//   // randomSparkles();
//   RgbwColor color = Hsvw2Rgbw(color_picker, 1.0, 0.5, 0);//poti_b_val, 0);
//   addFloat(pos, pos + block_len, color);


//   // end check
//   if (
//     (pos + block_len > end_pos   && dir ==  1) ||   //
//     (pos             < start_pos && dir == -1)) {   //
//     dir *= -1;
//   }

//   // update position
//   pos += speed * dir;

//   color_picker += color_delta;
//   if (color_picker > 360)
//     color_picker -= 360;

// }

// /** ====================================================================
//   BouncingBall

//   Not-so-realistic bouncing ball effect that changes in color.
//   Bouncing behavour based on equation:
//     y = abs( _h * e^( _d * x ) * cos( _w * x^2 ))
// */
// class BouncingBall {
//   private:
//     // changing variables
//     float _pos;                   // current position
//     float _t;                     // current time
//     int _dir = 1;                 // current direction
//     float _color_hue;             // current hue color
//     float _h;                     // height from _start_pos to _end_pos

//     // "constant" variables which are technically not constant
//     // because C++ does not allow for arbitrary optional params to be set. thanks for that.
//     float _d;                     // dampening factor
//     float _f;                     // frequency modifier
//     float _start_point;           // start point
//     float _end_point;             // end point
//     float _step;                  // step size between updates
//     float _size;                  // ball size
//     float _color_delta;           // color change per update
//     float _time_max;              // time until animation reverse
//     bool _reversed;               // direction of bounce

//   public:
//     /** ----------------------------------------------------------------
//       Constructor
//     */
//     BouncingBall()
//     {
//       // initialize default values
//       // C++ does not let you choose an optional parameter to set, so we have to go this way
//       _d = 0.05;
//       _f = 0.01;
//       _start_point = 0;
//       _end_point = NUM_LEDs;
//       _step = 0.1;
//       _size = 2.0;
//       _color_delta  = 0.7;
//       _time_max = 40;
//       _reversed = false;
//       _pos = random(_start_point, _end_point);
//       _t = random(_time_max);
//       _dir = random(2) == 0 ? -1 : 1;
//       _color_hue = random(360);
//       _h = _end_point - _start_point;
//     }

//     // -----------------------------------------------------------------
//     // update / draw

//     /**
//        update

//        Advance the current state by one step.
//     */
//     void update() {

//       // get position
//       _pos = abs(_h * exp(-_d * _t) * cos(_f * (_t * _t)));
//       if (_reversed)
//         _pos = _end_point - _pos;

//       // update params
//       _t += _step * _dir;
//       _color_hue += _color_delta;
//       if (_color_hue > 360) {
//         _color_hue -= 360;
//       }

//       // reverse animation
//       if (_t > _time_max || _t < 0) {
//         _dir *= -1;
//       }

//     }

//     /**
//       draw

//       Draw the current state to the led strip.
//     */
//     void draw() {
//       // float brightness = get_poti(BRIGHTNESS);
//       addFloat(_pos, _pos + _size, Hsvw2Rgbw(_color_hue, 1, BRIGHTNESS, 0));
//     }


//     // -----------------------------------------------------------------
//     // setters for "constant" variables.
//     // These would be passed to the constructor, but C++ does not let you
//     // set arbitrary optional parameters. So this is the least bad

//     void set_dampening(float d) {
//       _d = d;
//     }

//     void set_frequency(float f) {
//       _f = f;
//     }

//     void set_start_point(float pos) {
//       _start_point = pos;
//     }

//     void set_end_point(float pos) {
//       _end_point = pos;
//     }

//     void set_step(float step) {
//       _step = step;
//     }

//     void set_size(float size) {
//       _size = size;
//     }

//     void set_color_delta(float delta) {
//       _color_delta = delta;
//     }

//     void set_time_max(float time_max) {
//       _time_max = time_max;
//     }

//     void set_reversed(bool reversed) {
//       _reversed = reversed;
//     }

// };



// /** ====================================================================

// */
// class ColorWheel1D {
//   private:
//     float _r;               // raduis of wheel
//     float _m;               // mid point of wheel

//     float _rot;                   // current rotation
//     float _speed;                 // max speed
//     float _t;                     // time
//     float _dt;                    // time delta

//   public:
//     /* constructor */
//     ColorWheel1D(float r = NUM_LEDs / 2.0, float m = NUM_LEDs / 2.0, float speed = 10.0)
//       : _r(r), _m(m), _speed(speed) {
//       _rot = random(360);
//       _t = random(360);
//       _dt = random(200) / 20.0;
//     }


//     void refresh() {
//       float d = _dt; // * get_poti(MOD);
//       _t = (_t + d < 360) ? _t + d - 360 : _t + d;
//       _m = (sin(_t / 180 * PI) + 1) * NUM_LEDs / 2;
//       _rot = fmod(_rot + _speed, 360);
//     }

//     void draw() {
//       // fadeToBlackBy(4);
//       for (int i = 0; i < NUM_LEDs; i++) {

//         // outside wheel
//         if (abs(i - _m) > _r)
//           continue;

//         float h = _rot;//cos(_pos/180 * PI)*360;

//         if (i - _m < 0) {
//           h = (h + 180) > 360 ? h - 180 : h + 180;
//         }
//         float s = abs(i - _m) / _r;
//         s *= 1.1;   // make the ends c o l o r f u l
//         float v = BRIGHTNESS / 2;
//         RgbwColor col = Hsvw2Rgbw(h, s, v, 0);
//         addPixel(i, col);
//         //strip.SetPixelColor(i, col);
//       }
//     }

// };



// /* =====================================================================
//   march_edges animation

//   March along edges of the lamp. This construction is tied to my physical lamp.
// */


// /**
//    Edge class used for marching_edge().
// */
// class Edge {
//   public:
//     const int a, b; // start & end pixel
//     const int va, vb; // start & end vertex
//     Edge(int a_, int b_, int va_, int vb_) : a(a_), b(b_), va(va_), vb(vb_) {}

// };

// /**
//    Vertex class used for march_edges().
// */
// class Vertex {
//   public:
//     const string id;
//     const vector<int> edges;
//     const int num_edges;

//     Vertex(string identifier, vector<int> es) : id(identifier), edges(es), num_edges(edges.size()) {
//     }

//     int get_random_edge() {
//       return edges[random(num_edges)];
//     }

//     int get_random_edge_but_not(int e) {
//       int idx = random(num_edges);
//       if (edges[idx] == e) {
//         idx = (idx + 1) % num_edges;
//       }
//       return edges[idx];
//     }

//     bool contains(int n) {
//       return (find(edges.begin(), edges.end(), n) != edges.end());
//     }

// };

// /*
//    march_edges
// */
// void march_edges() {

//   static Edge E[25] {
//     {0, 21, 8, 0},      {22, 35, 0, 4},     {36, 48, 4, 8},     {49, 61, 8, 9},     {62, 72, 9, 4},
//     {73, 91, 4, 7},     {92, 103, 7, 9},    {104, 128, 9, 10},  {129, 140, 10, 7},  {141, 163, 7, 6},
//     {164, 175, 6, 10},  {176, 189, 10, 11}, {190, 198, 11, 6},  {199, 210, 6, 3},   {211, 232, 3, 11},
//     {233, 243, 3, 2},   {244, 257, 2, 6},   {258, 275, 6, 5},   {276, 288, 5, 2},   {289, 315, 2, 1},
//     {316, 332, 1, 5},   {333, 343, 5, 7},   {344, 365, 5, 4},   {366, 376, 4, 1},   {377, 391, 1, 0}
//   };

//   static Vertex V[12] {
//     {"A", vector<int>{0, 1, 24}},
//     {"B", vector<int>{19, 20, 23, 24}},
//     {"C", vector<int>{15, 16, 18, 19}},
//     {"D", vector<int>{13, 14, 15}},
//     {"E", vector<int>{1, 2, 4, 5, 22, 23}},
//     {"F", vector<int>{17, 18, 20, 21, 22}},
//     {"G", vector<int>{9, 10, 12, 13, 16, 17}},
//     {"H", vector<int>{5, 6, 8, 9, 21}},
//     {"I", vector<int>{0, 2, 3}},
//     {"J", vector<int>{3, 4, 6, 7}},
//     {"K", vector<int>{7, 8, 10, 11}},
//     {"L", vector<int>{11, 12, 14}}
//   };
//   static float h = random(360);

//   static int last_edge = -1;
//   static int last_pixel = -1;
//   static int dir; // 0 = down, 1 = up




//   int current_edge;
//   // choose new edge
//   if (last_edge == -1) {
//     // choose random edge + direction
//     current_edge = random(sizeof(E) / sizeof(*E));
//     dir = random(2);

//   } else {
//     // not initial

//     // get current vertex
//     int verts = sizeof(V) / sizeof(*V);
//     for (int i = 0; i < verts; i++) {
//       if (V[i].contains(last_edge) && (dir == 0 ? i == E[last_edge].va : i == E[last_edge].vb)) {
//         // choose random edge at vertex

//         current_edge = V[i].get_random_edge_but_not(last_edge);

//         // decide whether to go up or down the edge
//         if (dir > 0) {
//           // were going up -> last end vert = new start vert
//           if (E[last_edge].vb == E[current_edge].va) {
//             // go up
//             dir = 1;
//           } else {
//             // go down
//             dir = 0;
//           }
//         } else {
//           // were going down -> last start vert = new start vert
//           if (E[last_edge].va == E[current_edge].va) {
//             // go up
//             dir = 1;
//           } else {
//             // go down
//             dir = 0;
//           }
//         }
//         i = verts;
//         break;
//       }
//     }
//   }


//   // animation time!

//   // iterate over edge
//   int len = abs(E[current_edge].b - E[current_edge].a);
//   for (int i = 0; i <= len; i++) {

//     // update position
//     int pos = i;
//     if (dir == 0) {
//       pos = len - i;
//     }
//     pos += E[current_edge].a;

//     // display
//     //fadeToBlackBy(16 * get_poti(MOD));
//     addPixel(pos, Hsvw2Rgbw(h, 1, 0.5, 0));

//     // update color
//     h += 4.4;
//     if (h > 360) {
//       h -= 360;
//     }

//     // save the last pixel
//     if (i == len) {
//       last_pixel = pos;
//     }

//     fadeToBlackBy(16);
//     delay(20);
//     show();
//   }
//   // save the last edge
//   last_edge = current_edge;


// }


// void diag_bars() {
//   static int x = random(lamp_x);
//   static int _x_dir = 1;
//   static int _dx = 20;

//   static int y = random(lamp_x);
//   static int _y_dir = 1;
//   static int _dy = 20;

//   fadeToBlackBy(10);

//   static float _hue_x = random(360);
//   static float _hue_y = random(360);

//   for (int i = 0; i < NUM_LEDs; i++) {
//     if (abs((lamp[i].x + lamp[i].y) - x) < _dx) {
//       // addPixel(i, Hsvw2Rgbw(_hue_x, 1, get_poti(MOD), 0));
//     }
//     if (abs((lamp[i].x - lamp[i].y) - y) < _dy) {
//       // addPixel(i, Hsvw2Rgbw(_hue_y, 1, get_poti(MOD), 0));
//     }
//   }

//   if (x >= lamp_x + lamp_y || x < 0) {
//     _x_dir *= -1;
//     x += 10 * _x_dir;
//     _hue_x = random(360);
//   }
//   x += 10 * _x_dir;
//   _hue_x += 0.5;
//   if (_hue_x > 360) {
//     _hue_x -= 360;
//   }

//   if (y >= lamp_x || y < -lamp_y) {
//     _y_dir *= -1;
//     y += 10 * _y_dir;
//     _hue_y = random(360);
//   }
//   y += 12 * _y_dir;
//   _hue_y += 0.5;
//   if (_hue_y > 360) {
//     _hue_y -= 360;
//   }
// }


// void sine_wave() {
//   static float tns = 0.3;
//   static float h = 0;

//   static float t = 0;
//   clear_strip();
//   for (int i = 0; i < NUM_LEDs; i++) {
//     float x = lamp[i].x / 250.0 + t;
//     float y = ((float) lamp[i].y / lamp_y) * 2 - 1;
//     if ( abs((y) - (sin(x) * 1.03)) < tns) {
//       float x_add = (float) lamp[i].x / lamp_x * 360;
//       float hue = h + x_add > 360 ? h + x_add - 360 : h + x_add;
//       strip.SetPixelColor(i, Hsvw2Rgbw(h, 1, 1, 0));
//     }
//   }


//   t += 0.04;
//   h += 0.5;
//   if (h > 360) {
//     h -= 360;
//   }
// }
