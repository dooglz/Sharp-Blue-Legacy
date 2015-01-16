// THIS GAME FILE IS FOR DEBUGINNG THE ENGINE, PROJECT USING THE ENGINE WILL
// HAVE THEIR OWN GAME FILE

#include "Maths.h"
#include "Game.h"
#include "Mesh.h"
#include "torus.h"
#include "Input.h"
#include <string>
#include "Actor.h"
#include "GameEngine.h"

#include "Renderer.h"
#include "MeshLoader.h"
#include "Event_Manager.h"
#include "Font.h"

#if defined(_PS3_)
#include <sys/paths.h>
#endif

Matrix4 ViewProjection;
Matrix4 projMatrix;
Matrix4 ModelProjection1;
Matrix4 ModelProjection2;
stMesh* bmesh;
Engine::Actor torusActor;
stMesh torus;
Vector3 cameraPos;
float horizontalAngle;
float verticalAngle;

void registerInputs() {
  // Todo: read these from file
  // using pushback rather than a={...} as the ps3 compiler is old and dumb

  std::vector<std::string> a;

  a.push_back("kb_space");
  a.push_back("gp_1_start");
  a.push_back("ms_l");
  Engine::Input::addMap("action1", a);
  a.clear();

  a.push_back("kb_left");
  a.push_back("gp_1_x");
  Engine::Input::addMap("left", a);
  a.clear();
  a.push_back("kb_right");
  a.push_back("gp_1_b");
  Engine::Input::addMap("right", a);
  a.clear();
  a.push_back("kb_up");
  a.push_back("gp_1_y");
  Engine::Input::addMap("up", a);
  a.clear();
  a.push_back("kb_down");
  a.push_back("gp_1_a");
  Engine::Input::addMap("down", a);
  a.clear();

  a.push_back("kb_k_w");
  a.push_back("gp_1_dpad_u");
  Engine::Input::addMap("W", a);
  a.clear();
  a.push_back("kb_k_a");
  a.push_back("gp_1_dpad_l");
  Engine::Input::addMap("A", a);
  a.clear();
  a.push_back("kb_k_s");
  a.push_back("gp_1_dpad_d");
  Engine::Input::addMap("S", a);
  a.clear();
  a.push_back("kb_k_d");
  a.push_back("gp_1_dpad_r");
  Engine::Input::addMap("D", a);
  a.clear();

  /*	Input::addMap("left",	{"kb_left", "gp_1_dpad_left"});
          Input::addMap("right",	{"kb_right", "gp_1_dpad_right"});
          Input::addMap("up",		{ "kb_right", "gp_1_dpad_up" });
          Input::addMap("down",	{ "kb_down", "gp_1_dpad_down" });
          Input::addMap("action1",{ "kb_space", "gp_1_x", "ms_l" });*/
}

void Game::init() {

  // load a Torus mesh from the torus generator
  torus.vertexData = CreateTorus(1, 5, 10, 10);

  torus.loadedMain = true;
  torus.numVerts = torus.vertexData.size();
  torus.strip = true;
  Engine::Renderer->assignShader(&torus, "");
  Engine::GameEngine::Meshloader->loadOnGPU(&torus);

  // load a model from a .obj file
  std::string name = "models/beacon.obj";
  name = FILE_PATH + name;
  bmesh = Engine::GameEngine::Meshloader->loadOBJFile(name);
  Engine::Renderer->assignShader(bmesh, "");
  Engine::GameEngine::Meshloader->loadOnGPU(bmesh);

  // Setup view matricies
  // Projection matrix : 60° Field of View, 16:9 ratio, display range : 0.1 unit
  // <-> 100 units
  projMatrix = Perspective((float)(60.0f * (M_PI / 180.0f)), (16.0f / 9.0f),1.0f, 2000.0f);

  registerInputs();

  horizontalAngle = 1.5f;
  cameraPos = Vector3(-30, 0, 0);
  verticalAngle = 0.0f;

  torusActor.scale = Vector3(2.0f, 2.0f, 2.0f);
  torusActor.rotation = Vector3(0.0f, 0.0f, 0.0f);
  torusActor.position = Vector3(0.0f, 0.0f, 0.0f);
}
float a;
float x, y, z;
void Game::update(double delta) {
  a += 0.01f;
  torusActor.rotation = Vector3(0, a * 0.1f, 0);
  if (Engine::Input::getMapData("action1") > 128) {
    printf("action pressed\n");
  }
  if (Engine::Input::getMapData("left") > 128) {
    horizontalAngle += 0.01f;
  }
  if (Engine::Input::getMapData("right") > 128) {
    horizontalAngle -= 0.01f;
  }
  if (Engine::Input::getMapData("up") > 128) {
    verticalAngle += 0.01f;
  }
  if (Engine::Input::getMapData("down") > 128) {
    verticalAngle -= 0.01f;
  }

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  Vector3 direction =
      Vector3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle),
              cos(verticalAngle) * cos(horizontalAngle));

  // Right vector
  Vector3 right = Vector3(sin(horizontalAngle - 3.14f / 2.0f), 0,
                          cos(horizontalAngle - 3.14f / 2.0f));

  // Up vector
  Vector3 up = Cross(right, direction);

  if (Engine::Input::getMapData("W") > 128) {
    cameraPos += (float)(delta / 100.0) * direction;
  }
  if (Engine::Input::getMapData("A") > 128) {
    cameraPos += (float)(delta / -100.0) * right;
  }
  if (Engine::Input::getMapData("S") > 128) {
    cameraPos += (float)(delta / -100.0) * direction;
  }
  if (Engine::Input::getMapData("D") > 128) {
    cameraPos += (float)(delta / 100.0) * right;
  }

  Matrix4 viewMatrix = Lookat(cameraPos, cameraPos + direction, up);
  ViewProjection = (projMatrix * viewMatrix);

  Matrix4 rot = AngleAxisToMatrix(Vector3(0, 1, 0), a * 0.1f* delta);
  Matrix4 rot2 = AngleAxisToMatrix(Vector3(0, 1, 0), a * -1.0f * delta);
  Matrix4 scl = Scale(Vector3(2.0f, 2.0f, 2.0f));
  // glm rotation uses degrees
  ModelProjection1 =
      ViewProjection * Translation(Vector3(0, 0, 0)) * scl * rot2;
  ModelProjection2 =
      ViewProjection * Translation(Vector3(0, 0, 0)) * scl * rot;
}

bool flp;
void Game::render() {
  flp = !flp;
  Engine::Renderer->RenderMesh(bmesh, ModelProjection2);
  // Engine::Engine::Renderer->renderMesh(&torus, ModelProjection1);
  Engine::Renderer->RenderMesh(
      &torus, ViewProjection * torusActor.getModelProjection());
  Engine::GameEngine::Font->renderString(flp, "Hello World", 150, 150);
}

void Game::shutdown() {}
