#include "Utilities.h" // asserts and commmon defines
#include "GameEngine.h"
#include <time.h>
#include <math.h>
//#include <stdio.h>

#include <stdlib.h>

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
#include "PC/OGL_Meshloader.h"
#include "PC/OGL_Renderer.h"
#include "PC/OGL_Meshloader.h"
#include "PC/SDL_Event_Manager.h"
#include "PC/SDL_Font.h"

SDL_assert_state CustomAssertionHandler(const SDL_assert_data* data,
                                        void* userdata) {
  // CheckGL();
  // CheckSDL();
  // ASSERT_MSG((err == 0), "TTF_Init() error");
  SDL_AssertionHandler defaultHandler = SDL_GetDefaultAssertionHandler();
  return defaultHandler(data, userdata);
}

#endif

namespace Engine {
bool GameEngine::run;
Renderer* GameEngine::Renderer;
Meshloader* GameEngine::Meshloader;
EventManager* GameEngine::EventManager;
Font* GameEngine::Font;
void (*GameEngine::_GameUpdate)(double);
void (*GameEngine::_GameRender)();

void GameEngine::init() {
  // Seed random generator
  srand((unsigned int)time(NULL));

// load relevant modules
#if defined(_vita_)
  GameEngine::Renderer = new GXM::GXM_Renderer();
  GameEngine::Meshloader = new GXM::GXM_Meshloader();
  GameEngine::EventManager = new VITA::VITA_EventManager();
  GameEngine::Font = new VITA::VITA_Font();
#elif defined(_PS3_)
  GameEngine::Renderer = new GCM::GCM_Renderer();
  GameEngine::Meshloader = new GCM::GCM_Meshloader();
  GameEngine::EventManager = new PS3::PS3EventManager();
  GameEngine::Font = new PS3::PS3_Font();
#elif defined(_WINDOWS_)

  GameEngine::Renderer = new OGL::OGL_Renderer();
  GameEngine::Meshloader = new OGL::OGL_Meshloader();
  GameEngine::EventManager = new OGL::SDLEventManager();
  GameEngine::Font = new SDL_Font();
#endif

  // init modules
  GameEngine::Renderer->init();
  GameEngine::EventManager->init();
  GameEngine::Font->init();
  GameEngine::run = true;
}

void GameEngine::update(const double delta) { EventManager->processEvents(); }

void GameEngine::render() {
  // Prep for render
  GameEngine::Renderer->SetViewport();
  GameEngine::Renderer->setupFrame();
  GameEngine::Renderer->clearSurface();
}
void GameEngine::Postrender() {
  GameEngine::Font->flush();
  GameEngine::Renderer->swapBuffers();
}

void GameEngine::shutdown() {
  GameEngine::Font->shutdown();
  delete GameEngine::Font;
  GameEngine::Font = NULL;

  delete GameEngine::EventManager;
  GameEngine::EventManager = NULL;

  delete GameEngine::Meshloader;
  GameEngine::Meshloader = NULL;

  delete GameEngine::Renderer;
  GameEngine::Renderer = NULL;
}

double GameEngine::getTime() {
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
  double currentTime = Engine::GameEngine::getTime();
  // 60fps in Microseconds.
  double tartgettime = 1.0 / 60.0;

  while (run) {
    double newTime = Engine::GameEngine::getTime();
    delta = newTime - currentTime;

    // delta / 60fps
    double deltaPercent = (double)delta / (double)tartgettime;

    // Update engine
    update(deltaPercent);

    // Update game
    _GameUpdate(deltaPercent);

    // Render
    render();
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
}