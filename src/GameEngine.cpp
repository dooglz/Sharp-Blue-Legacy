#include "Utilities.h" // asserts and commmon defines
#include "GameEngine.h"

#include <Rocket/Core.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "Scene.h"
#include "Event_Manager.h"

#include "Storage.h"


#if defined(_vita_)
#elif defined(_PS3_)
#include <sys/sys_time.h>
#elif defined(_WINDOWS_)
#include "sdl/SDL.h"
#include "PC/SDL_platform.h"
#include "PC/OGL_Renderer.h"

//#include "PC/SDL_Font.h"
#endif

namespace Engine {
bool GameEngine::run;

// Font* GameEngine::Font;
void (*GameEngine::_GameUpdate)(double);
void (*GameEngine::_GameRender)();

void GameEngine::Init(const unsigned short width, const unsigned short height) {

  // Seed random generator
  srand((unsigned int)time(NULL));

#if defined(_vita_)
#elif defined(_PS3_)
#elif defined(_WINDOWS_)
  Platform = new SDL::SDL_Platform();
#endif

  // init modules
  Platform->Init(width, height);
  // Loading scene
  ActiveScene = new Scene();
  run = true;
}

void GameEngine::SetActiveScene(Scene* scene) { ActiveScene = scene; }

void GameEngine::Shutdown() {
  ActiveScene->Shutdown();
  delete ActiveScene;
  ActiveScene = 0;

  Platform->Shutdown();
  delete Platform;
  Platform = 0;

  Base_storage::Shutdown();
}

void GameEngine::Loop() {
  double delta = 0;
  double currentTime = Platform->GetTime();
  // 60fps in Microseconds.
  double tartgettime = 1.0 / 60.0;

  while (run) {
    // continue;
    double newTime = Platform->GetTime();
    delta = newTime - currentTime;
    currentTime = newTime;

    // delta / 60fps
    double deltaPercent = (double)delta / (double)tartgettime;

    // Update engine
    EventManager->processEvents();
    ActiveScene->Update(deltaPercent);
    Rcontext->Update();

    // Update game
    _GameUpdate(deltaPercent);

    // Render
    Renderer->PrepFrame();
    ActiveScene->Render();

    _GameRender();
   // Rcontext->Render();

    Renderer->PostRender();
    // Finished Drawing
  }
};

void GameEngine::RegisterUpdate(void (*GameUpdate)(double)) {
  _GameUpdate = GameUpdate;
};

void GameEngine::RegisterRender(void (*GameRender)()) {
  _GameRender = GameRender;
};

void GameEngine::StopLoop() { run = false; }
}