/*
        ENGINE MAIN
        This is an example main, used for debugging the engine,
        games using the library will have their own main file.

        Sam Serrels
        sam@samserrels.com
*/
// Memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include "Utilities.h" // asserts and commmon defines
#include "GameEngine.h"
#include "Game.h"

// *************************
//
// Program Entry Point: main
//

int main(int argc, char **argv) {
#if leakdetect
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//_CrtSetBreakAlloc(276); //Use to break on alloc number e.g{123}
#endif
  puts("Program Entry Point: Main\n");

  Engine::GameEngine::Init(1280, 720);

  Game *GameCode = new Game();
  GameCode->init();

  Engine::GameEngine::RegisterUpdate(Game::update);
  Engine::GameEngine::RegisterRender(Game::render);

  puts("Starting loop\n");
  Engine::GameEngine::Loop();

  puts("Shutting Down\n");

  GameCode->shutdown();
  Engine::GameEngine::Shutdown();

  delete GameCode;
  GameCode = NULL;

  puts("Quitting\n");

  return 0;
}