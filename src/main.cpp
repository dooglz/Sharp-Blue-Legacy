/*
	ENGINE MAIN
	This is an example main, used for debugging the engine, 
	games using the library will have their own main file.

	Sam Serrels
	sam@samserrels.com
*/
#include <stdio.h>
#include "Utilities.h" // asserts and commmon defines
#include "GameEngine.h"
#include "Game.h"

// ******************************************************************************
//
// Program Entry Point: main 
//
int main(int argc, char** argv)
{

	puts("Program Entry Point: Main\n");

	Engine::GameEngine::init();

	Game * GameCode = new Game(); 

	GameCode->init();
	float delta = 0;

	puts("Starting loop\n");

	unsigned int prevtime = Engine::GameEngine::getTime();
	while (Engine::GameEngine::run)
	{	
		
		unsigned int time = Engine::GameEngine::getTime();

		//delta should be in milliseconds
		delta = ((float)(time - prevtime)) / (1000000.0f / 60.0f);
		prevtime = time;

		//Update engine 
		Engine::GameEngine::update(delta);

		//Update game
		GameCode->update(delta);
		
		//Render
		Engine::GameEngine::render();
		GameCode->render();

		// Finished Drawing
		Engine::GameEngine::Postrender();
	}

	puts("Shutting Down\n");

	GameCode->shutdown();
	Engine::GameEngine::shutdown();

	delete GameCode;
	GameCode = NULL;

	puts("Quitting\n");
	return 0;

}