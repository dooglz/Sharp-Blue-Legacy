#pragma once

namespace Engine {
class Renderer;
class Meshloader;
class EventManager;
class Font;

class GameEngine {

private:
  static void (*_GameUpdate)(float);
  static void (*_GameRender)();
  static bool run;

  static void update(const float delta);
  static void render();
  static void Postrender();

public:
  static void shutdown();
  static void init();
  static void Loop();
  static void StopLoop();
  static void RegisterUpdate(void (*GameUpdate)(float));
  static void RegisterRender(void (*GameRender)());
  //
  static unsigned int getTime();
  //
  static Renderer* Renderer;
  static Meshloader* Meshloader;
  static EventManager* EventManager;
  static Font* Font;
};
}