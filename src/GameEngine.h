#pragma once

namespace Engine {
class Meshloader;
class EventManager;

class GameEngine {

private:
  static void(*_GameUpdate)(double);
  static void (*_GameRender)();
  static bool run;
public:
  static void Shutdown();
  static void Init(const unsigned short width, const unsigned short height);
  static void Loop();
  static void StopLoop();
  static void RegisterUpdate(void(*GameUpdate)(double));
  static void RegisterRender(void (*GameRender)());
};
}