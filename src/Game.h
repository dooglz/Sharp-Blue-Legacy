//THIS GAME FILE IS FOR DEBUGINNG THE ENGINE, PROJECT USING THE ENGINE WILL HAVE THEIR OWN GAME FILE

#pragma once
#include "Utilities.h"	
class Game{
private:

public:
	void init();
	void update(float delta);
	void render();
	void shutdown();
	Game(){};

};