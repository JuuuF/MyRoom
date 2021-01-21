
#pragma once
#include "animations.h"
#include "utils.h"
#include "led_functions.h"

extern const int NUM_LEDs;
extern int SCENE;
extern int SCENE_COUNT;

class Scene {
  private:


  public:

};

/**---------------------------------------------------------------------
  set_scene

  Set the current cene number depending on button inputs.

  Sets:
    int SCENE                   currently selected scene.

  Returns:
    -1                          scene count decreased
    0                           nothing changed
    +1                          scene increased
*/
int set_scene() {

  int prev = SCENE;
  
  if (get_button(LEFT_BUTTON, FALLING_EDGE)) {
    // left button
    SCENE--;
    SCENE = constrain(SCENE, 0, SCENE_COUNT - 1);
    return prev == SCENE ? 0 : -1;
  }
  if (get_button(RIGHT_BUTTON, FALLING_EDGE)) {
    // right button
    SCENE++;
    SCENE = constrain(SCENE, 0, SCENE_COUNT - 1);
    return prev == SCENE ? 0 : 1;
  }
  
  return 0;
}
