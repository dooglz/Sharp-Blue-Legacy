#include "Utilities.h" // asserts and commmon defines
#include "GameEngine.h"
#include <time.h>
#include <math.h>
//#include <stdio.h>

#include <stdlib.h>

#include "Scene.h"
#include "platform.h"

#if defined(_vita_)
#include "VITA/GXM_Renderer.h"
#include "VITA/GXM_Meshloader.h"
#include "VITA/VITA_Font.h"
#include "VITA/VITA_Event_Manager.h"
#elif defined(_PS3_)
#include <sys/sys_time.h>
#include "PS3/GCM_Meshloader.h"
#include "PS3/GCM_Renderer.h"
#include "PS3/GCM_Meshloader.h"
#include "PS3/PS3_Event_Manager.h"
#include "PS3/PS3_Font.h"
#elif defined(_WINDOWS_)
#include "sdl/SDL.h"
#include "PC/SDL_platform.h"
#include "PC/OGL_Meshloader.h"
#include "PC/OGL_Renderer.h"
#include "PC/OGL_Meshloader.h"
#include "PC/SDL_Event_Manager.h"
#include "PC/SDL_Font.h"



#endif

namespace Engine {
bool GameEngine::run;
CPlatform* Platform = 0;
Meshloader* GameEngine::Meshloader;
EventManager* GameEngine::EventManager;
Font* GameEngine::Font;
void (*GameEngine::_GameUpdate)(double);
void (*GameEngine::_GameRender)();

void GameEngine::Init() {
  // Seed random generator
  srand((unsigned int)time(NULL));

#if defined(_vita_)
  Renderer = new GXM::GXM_Renderer();
  Meshloader = new GXM::GXM_Meshloader();
  EventManager = new VITA::VITA_EventManager();
  Font = new VITA::VITA_Font();

#elif defined(_PS3_)

  Renderer = new GCM::GCM_Renderer();
  Meshloader = new GCM::GCM_Meshloader();
  EventManager = new PS3::PS3EventManager();
  Font = new PS3::PS3_Font();

#elif defined(_WINDOWS_)

  Renderer = new OGL::OGL_Renderer();
  Meshloader = new OGL::OGL_Meshloader();
  EventManager = new OGL::SDLEventManager();
  //Platform = new SDL::SDL_Platform();
  Font = new SDL_Font();
#endif

  // init modules
  Renderer->Init();
  EventManager->init();
  Font->init();
  ActiveScene = new Scene();

  run = true;
}

void GameEngine::Update(const double delta) {
  EventManager->processEvents();
  ActiveScene->Update(delta);
}

void GameEngine::Render() {
  // Prep for render
  Renderer->SetViewport();
  Renderer->SetupFrame();
  Renderer->ClearSurface();
}
void GameEngine::Postrender() {
  Font->flush();
  Renderer->SwapBuffers();
}

void GameEngine::Shutdown() {
  ActiveScene->Shutdown();
  delete ActiveScene;

  Font->shutdown();
  delete Font;
  Font = NULL;

  delete EventManager;
  EventManager = NULL;

  delete Meshloader;
  Meshloader = NULL;

  delete Renderer;
  Renderer = NULL;
}

double GameEngine::GetTime() {
#if defined(_PS3_)
  // microseconds
  return (((double)(sys_time_get_system_time())) * 0.000001);
#elif defined(_WINDOWS_)
  return ((double)(SDL_GetPerformanceCounter()) /
          ((double)SDL_GetPerformanceFrequency()));
#endif
  return 0;
}

void GameEngine::Loop() {
  double delta = 0;
  double currentTime = Engine::GameEngine::GetTime();
  // 60fps in Microseconds.
  double tartgettime = 1.0 / 60.0;

  while (run) {
      continue;
    double newTime = Engine::GameEngine::GetTime();
    delta = newTime - currentTime;

    // delta / 60fps
    double deltaPercent = (double)delta / (double)tartgettime;

    // Update engine
    Update(deltaPercent);

    // Update game
    _GameUpdate(deltaPercent);

    // Render
    Render();
    _GameRender();

    // Finished Drawing
    Postrender();
  }
};

void GameEngine::RegisterUpdate(void (*GameUpdate)(double)) {
  _GameUpdate = GameUpdate;
};

void GameEngine::RegisterRender(void (*GameRender)()) {
  _GameRender = GameRender;
};

void GameEngine::StopLoop() { run = false; }


void GameEngine::CreateWindow(unsigned int x, unsigned int y)
{    
    Platform->InitDisplay(x, y);
}

}