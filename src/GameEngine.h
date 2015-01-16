#pragma once

namespace Engine {
class Meshloader;
class EventManager;
class Font;

class GameEngine {

private:
  static void(*_GameUpdate)(double);
  static void (*_GameRender)();
  static bool run;

  static void Update(const double delta);
  static void Render();
  static void Postrender();

public:
  static void Shutdown();
  static void Init();
  static void Loop();
  static void StopLoop();
  static void RegisterUpdate(void(*GameUpdate)(double));
  static void RegisterRender(void (*GameRender)());
  static void CreateWindow(unsigned int x, unsigned int y);
  //
  static double GetTime();
  //

  static Meshloader* Meshloader;
  static EventManager* EventManager;
  static Font* Font;
};
}