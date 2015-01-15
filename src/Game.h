// THIS GAME FILE IS FOR DEBUGINNG THE ENGINE, PROJECT USING THE ENGINE WILL
// HAVE THEIR OWN GAME FILE

#pragma once
#include "Utilities.h"
class Game {
private:
public:
  static void init();
  static void update(double delta);
  static void render();
  void shutdown();
  Game(){};
};