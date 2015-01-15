#pragma once

namespace Engine {
class Renderer;
class Meshloader;
class EventManager;
class Font;

class GameEngine {

private:
  static void(*_GameUpdate)(float);
  static void(*_GameRender)();
public:
  static void Loop();
  static void RegisterUpdate(void(*GameUpdate)(float));
  static void RegisterRender(void(*GameRender)());

  //
  static bool run;
  static void init();
  static void update(const float delta);
  static void render();
  static void Postrender();
  static void shutdown();
  //
  static unsigned int getTime();
  //
  static Renderer* Renderer;
  static Meshloader* Meshloader;
  static EventManager* EventManager;
  static Font* Font;
};
}