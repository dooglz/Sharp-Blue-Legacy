#pragma once

namespace Engine {
class Renderer;
class Meshloader;
class EventManager;
class Font;

class GameEngine {

private:
  static void(*_GameUpdate)(double);
  static void (*_GameRender)();
  static bool run;

  static void update(const double delta);
  static void render();
  static void Postrender();

public:
  static void shutdown();
  static void init();
  static void Loop();
  static void StopLoop();
  static void RegisterUpdate(void(*GameUpdate)(double));
  static void RegisterRender(void (*GameRender)());
  //
  static double getTime();
  //
  static Renderer* Renderer;
  static Meshloader* Meshloader;
  static EventManager* EventManager;
  static Font* Font;
};
}