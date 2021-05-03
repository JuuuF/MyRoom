
//----------------------------------------------------------------
// animations

#pragma once

#include "led_functions.h"
#include "utils.h"
#include "pixels.h"

using namespace std;

// util variables
extern const int NUM_LEDs;

// lamp variables
extern Pixel lamp[];
extern int lamp_x, lamp_y;

// enumerations
extern const byte BRIGHTNESS, MOD, LEFT_BUTTON, RIGHT_BUTTON;


void random_function() {

}

void draw() {
  static int _t = 0;
  _t++;
  _t %= NUM_LEDs;
  fadeToBlackBy(16);
  setPixelColor(_t, RgbwColor(64));
}

class Animation {

  public:
    void(*update)();            // update the animation state
    void(*draw)();              // draw the animation

    Animation() {}

    Animation(void(*update_function)()/*, void(*draw_function)()*/)
      : update(update_function) {
      /*, draw(draw_function)*/
    }

};

class Test_animation { /*: Animation*/
  private:
    int _t;

  public:

    void update_anim() {
      _t++;
      _t %= NUM_LEDs;
    }

    void draw_anim() {
      fadeToBlackBy(16);
      setPixelColor(_t, RgbwColor(64));
    }

    Test_animation() {
      //Animation(draw);
      _t = random(NUM_LEDs);
    }
};






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


    void refresh() {
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



/* =====================================================================
  march_edges animation

  March along edges of the lamp. This construction is tied to my physical lamp.
*/


/**
   Edge class used for marching_edge().
*/
class Edge {
  public:
    const int a, b; // start & end pixel
    const int va, vb; // start & end vertex
    Edge(int a_, int b_, int va_, int vb_) : a(a_), b(b_), va(va_), vb(vb_) {}

};

/**
   Vertex class used for march_edges().
*/
class Vertex {
  public:
    const string id;
    const vector<int> edges;
    const int num_edges;

    Vertex(string identifier, vector<int> es) : id(identifier), edges(es), num_edges(edges.size()) {
    }

    int get_random_edge() {
      return edges[random(num_edges)];
    }

    int get_random_edge_but_not(int e) {
      int idx = random(num_edges);
      if (edges[idx] == e) {
        idx = (idx + 1) % num_edges;
      }
      return edges[idx];
    }

    bool contains(int n) {
      return (find(edges.begin(), edges.end(), n) != edges.end());
    }

};

/*
   march_edges
*/
void march_edges() {

  static Edge E[25] {
    {0, 21, 8, 0},      {22, 35, 0, 4},     {36, 48, 4, 8},     {49, 61, 8, 9},     {62, 72, 9, 4},
    {73, 91, 4, 7},     {92, 103, 7, 9},    {104, 128, 9, 10},  {129, 140, 10, 7},  {141, 163, 7, 6},
    {164, 175, 6, 10},  {176, 189, 10, 11}, {190, 198, 11, 6},  {199, 210, 6, 3},   {211, 232, 3, 11},
    {233, 243, 3, 2},   {244, 257, 2, 6},   {258, 275, 6, 5},   {276, 288, 5, 2},   {289, 315, 2, 1},
    {316, 332, 1, 5},   {333, 343, 5, 7},   {344, 365, 5, 4},   {366, 376, 4, 1},   {377, 391, 1, 0}
  };

  static Vertex V[12] {
    {"A", vector<int>{0, 1, 24}},
    {"B", vector<int>{19, 20, 23, 24}},
    {"C", vector<int>{15, 16, 18, 19}},
    {"D", vector<int>{13, 14, 15}},
    {"E", vector<int>{1, 2, 4, 5, 22, 23}},
    {"F", vector<int>{17, 18, 20, 21, 22}},
    {"G", vector<int>{9, 10, 12, 13, 16, 17}},
    {"H", vector<int>{5, 6, 8, 9, 21}},
    {"I", vector<int>{0, 2, 3}},
    {"J", vector<int>{3, 4, 6, 7}},
    {"K", vector<int>{7, 8, 10, 11}},
    {"L", vector<int>{11, 12, 14}}
  };
  static float h = random(360);

  static int last_edge = -1;
  static int last_pixel = -1;
  static int dir; // 0 = down, 1 = up




  int current_edge;
  // choose new edge
  if (last_edge == -1) {
    // choose random edge + direction
    current_edge = random(sizeof(E) / sizeof(*E));
    dir = random(2);

  } else {
    // not initial

    // get current vertex
    int verts = sizeof(V) / sizeof(*V);
    for (int i = 0; i < verts; i++) {
      if (V[i].contains(last_edge) && (dir == 0 ? i == E[last_edge].va : i == E[last_edge].vb)) {
        // choose random edge at vertex

        current_edge = V[i].get_random_edge_but_not(last_edge);

        // decide whether to go up or down the edge
        if (dir > 0) {
          // were going up -> last end vert = new start vert
          if (E[last_edge].vb == E[current_edge].va) {
            // go up
            dir = 1;
          } else {
            // go down
            dir = 0;
          }
        } else {
          // were going down -> last start vert = new start vert
          if (E[last_edge].va == E[current_edge].va) {
            // go up
            dir = 1;
          } else {
            // go down
            dir = 0;
          }
        }
        i = verts;
        break;
      }
    }
  }


  // animation time!

  // iterate over edge
  int len = abs(E[current_edge].b - E[current_edge].a);
  for (int i = 0; i <= len; i++) {

    // update position
    int pos = i;
    if (dir == 0) {
      pos = len - i;
    }
    pos += E[current_edge].a;

    // display
    //fadeToBlackBy(16 * get_poti(MOD));
    addPixelColor(pos, Hsvw2Rgbw(h, 1, 0.5, 0));

    // update color
    h += 4.4;
    if (h > 360) {
      h -= 360;
    }

    // save the last pixel
    if (i == len) {
      last_pixel = pos;
    }

    fadeToBlackBy(16);
    delay(20);
    show();
  }
  // save the last edge
  last_edge = current_edge;


}


void diag_bars() {
  static int x = random(lamp_x);
  static int x_dir = 1;
  static int dx = 20;

  static int y = random(lamp_x);
  static int y_dir = 1;
  static int dy = 20;

  fadeToBlackBy(10);

  static float h_x = random(360);
  static float h_y = random(360);

  for (int i = 0; i < NUM_LEDs; i++) {
    if (abs((lamp[i].x + lamp[i].y) - x) < dx) {
      addPixelColor(i, Hsvw2Rgbw(h_x, 1, get_poti(MOD), 0));
    }
    if (abs((lamp[i].x - lamp[i].y) - y) < dy) {
      addPixelColor(i, Hsvw2Rgbw(h_y, 1, get_poti(MOD), 0));
    }
  }

  if (x >= lamp_x + lamp_y || x < 0) {
    x_dir *= -1;
    x += 10 * x_dir;
    h_x = random(360);
  }
  x += 10 * x_dir;
  h_x += 0.5;
  if (h_x > 360) {
    h_x -= 360;
  }

  if (y >= lamp_x || y < -lamp_y) {
    y_dir *= -1;
    y += 10 * y_dir;
    h_y = random(360);
  }
  y += 12 * y_dir;
  h_y += 0.5;
  if (h_y > 360) {
    h_y -= 360;
  }
}


void sine_wave() {
  static float tns = 0.3;
  static float h = 0;

  static float t = 0;
  clear_strip();
  for (int i = 0; i < NUM_LEDs; i++) {
    float x = lamp[i].x / 250.0 + t;
    float y = ((float) lamp[i].y / lamp_y) * 2 - 1;
    if ( abs((y) - (sin(x) * 1.03)) < tns) {
      float x_add = (float) lamp[i].x / lamp_x * 360;
      float hue = h + x_add > 360 ? h + x_add - 360 : h + x_add;
      strip.SetPixelColor(i, Hsvw2Rgbw(h, 1, 1, 0));
    }
  }


  t += 0.04;
  h += 0.5;
  if (h > 360) {
    h -= 360;
  }
}
