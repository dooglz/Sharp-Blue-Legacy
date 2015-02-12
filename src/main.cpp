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
/*
struct AtExit
{
  ~AtExit() { _CrtDumpMemoryLeaks(); }
} doAtExit;
*/

int main(int argc, char **argv) {
  _CrtMemState s;
#if defined(leakdetect)
  _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 // _CrtSetBreakAlloc(780);
#endif
  _CrtMemCheckpoint(&s);
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

//  _CrtDumpMemoryLeaks();
  _CrtMemDumpAllObjectsSince(&s);
  return 0;
}