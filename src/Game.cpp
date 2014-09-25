//THIS GAME FILE IS FOR DEBUGINNG THE ENGINE, PROJECT USING THE ENGINE WILL HAVE THEIR OWN GAME FILE

#include "GMaths.h"
#include "Game.h"
#include "Mesh.h"
#include "torus.h"
#include "Input.h"
#include <string>
#include "Actor.h"
#include "GameEngine.h"	

#if defined(_PS3_)
	#include <sys/paths.h>
#endif

matrix4 ViewProjection;
matrix4 projMatrix;
matrix4 ModelProjection1;
matrix4 ModelProjection2;
stMesh* bmesh;
Engine::Actor torusActor;
stMesh torus;
vector3 cameraPos; 
float horizontalAngle;
float verticalAngle;

void registerInputs()
{
	//Todo: read these from file
	//using pushback rather than a={...} as the ps3 compiler is old and dumb

	std::vector<std::string> a;

	a.push_back("kb_space"); a.push_back("gp_1_start"); a.push_back("ms_l");
	Engine::Input::addMap("action1", a);
	a.clear();

	a.push_back("kb_left"); a.push_back("gp_1_x");
	Engine::Input::addMap("left", a);
	a.clear();
	a.push_back("kb_right"); a.push_back("gp_1_b");
	Engine::Input::addMap("right", a);
	a.clear();
	a.push_back("kb_up"); a.push_back("gp_1_y");
	Engine::Input::addMap("up", a);
	a.clear();
	a.push_back("kb_down"); a.push_back("gp_1_a");
	Engine::Input::addMap("down", a);
	a.clear();
	
	a.push_back("kb_k_w"); a.push_back("gp_1_dpad_u");
	Engine::Input::addMap("W", a);
	a.clear();
	a.push_back("kb_k_a"); a.push_back("gp_1_dpad_l");
	Engine::Input::addMap("A", a);
	a.clear();
	a.push_back("kb_k_s"); a.push_back("gp_1_dpad_d");
	Engine::Input::addMap("S", a);
	a.clear();
	a.push_back("kb_k_d"); a.push_back("gp_1_dpad_r");
	Engine::Input::addMap("D", a);
	a.clear();

/*	Input::addMap("left",	{"kb_left", "gp_1_dpad_left"});
	Input::addMap("right",	{"kb_right", "gp_1_dpad_right"});
	Input::addMap("up",		{ "kb_right", "gp_1_dpad_up" });
	Input::addMap("down",	{ "kb_down", "gp_1_dpad_down" });
	Input::addMap("action1",{ "kb_space", "gp_1_x", "ms_l" });*/
}


void Game::init()
{

	//load a Torus mesh from the torus generator	
	torus.vertexData = CreateTorus( 1, 5, 10, 10 );

	torus.loadedMain = true;
	torus.numVerts = torus.vertexData.size();
	torus.strip = true;
	Engine::GameEngine::Renderer->assignShader(&torus, "");
	Engine::GameEngine::Meshloader->loadOnGPU(&torus);

	
	//load a model from a .obj file
	std::string name = "models/beacon.obj";
	name = FILE_PATH + name ;
	bmesh = Engine::GameEngine::Meshloader->loadOBJFile(name);
	Engine::GameEngine::Renderer->assignShader(bmesh, "");
	Engine::GameEngine::Meshloader->loadOnGPU(bmesh);

	//Setup view matricies
	// Projection matrix : 60� Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
	projMatrix = M4::perspective(60.0f, (16.0f / 9.0f), 1.0f, 2000.0f);

	registerInputs();

	horizontalAngle = 1.5f;
	cameraPos = vector3(-30, 0, 0);
	verticalAngle = 0.0f;

	torusActor.scale = vector3(2.0f, 2.0f, 2.0f);
	torusActor.rotation = vector3(0.0f, 0.0f, 0.0f);
	torusActor.position = vector3(0.0f, 0.0f, 0.0f);
}
float a;
float x, y, z;
void Game::update(float delta)
{
	a += delta;
	torusActor.rotation = vector3(0, a*0.1f, 0);
	if (Engine::Input::getMapData("action1") > 128){
		printf("action pressed\n");
	}
	if (Engine::Input::getMapData("left") > 128){
		horizontalAngle += 0.01f;
	}
	if (Engine::Input::getMapData("right") > 128){
		horizontalAngle -= 0.01f;
	}	
	if (Engine::Input::getMapData("up") > 128){
		verticalAngle += 0.01f;
	}
	if (Engine::Input::getMapData("down") > 128){
		verticalAngle -= 0.01f;
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	vector3 direction = vector3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	vector3 right = vector3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	vector3 up = M3::cross(right, direction);

	if (Engine::Input::getMapData("W") > 128){
		cameraPos += (delta / 10.0f)*direction;
	}
	if (Engine::Input::getMapData("A") > 128){
		cameraPos += (delta / -10.0f)* right;
	}
	if (Engine::Input::getMapData("S") > 128){
		cameraPos += (delta / -10.0f)*direction;
	}
	if (Engine::Input::getMapData("D") > 128){
		cameraPos += (delta / 10.0f)* right;
	}

	matrix4 viewMatrix = M4::lookat(cameraPos, cameraPos + direction, up);
	ViewProjection = (projMatrix * viewMatrix);

	matrix4 rot = M4::rotation(a*0.025f , vector3(0,1,0));
	matrix4 rot2 = M4::rotation(a*-0.01f , vector3(0,1,0));
	matrix4 scl = M4::scale(vector3(2.0f, 2.0f, 2.0f));
	//glm rotation uses degrees
	ModelProjection1 = ViewProjection * M4::translation(vector3(0, 0, 0))* scl * rot2;
	ModelProjection2 = ViewProjection * M4::translation(vector3(0, 0, 0))* scl * rot;
}

bool flp;
void Game::render()
{
	flp = !flp;
	Engine::GameEngine::Renderer->renderMesh(bmesh, ModelProjection2);
	//Engine::Engine::Renderer->renderMesh(&torus, ModelProjection1);
	Engine::GameEngine::Renderer->renderMesh(&torus, ViewProjection * torusActor.getModelProjection());
	Engine::GameEngine::Font->renderString(flp, "Hello World", 150, 150);
}

void Game::shutdown()
{

}

