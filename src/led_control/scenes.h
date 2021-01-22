
#pragma once
#include "animations.h"
#include "utils.h"
#include "led_functions.h"

#include <vector>

extern const int NUM_LEDs;
extern int SCENE;
extern int SCENE_COUNT;

class Scene {
  public:
    const int number;
    //void (*update)();
    //const void *show;
    
    //Scene(int scene_number, void *scene_update)
    //  : number(scene_number), update(*scene_update) {}
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

  if (get_button(LEFT_BUTTON, FALLING_EDGE)) {
    // left button
    SCENE = (SCENE - 1) % SCENE_COUNT;
    return -1;
  }
  if (get_button(RIGHT_BUTTON, FALLING_EDGE)) {
    // right button
    SCENE = (SCENE + 1) % SCENE_COUNT;
    return 1;
  }
  
  return 0;
}
