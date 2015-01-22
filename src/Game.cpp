// THIS GAME FILE IS FOR DEBUGINNG THE ENGINE, PROJECT USING THE ENGINE WILL
// HAVE THEIR OWN GAME FILE

#include "Maths.h"
#include "Game.h"
#include "Input.h"

#include "GameEngine.h"
#include "Cm_MeshRenderer.h"
#include "Cm_FpsMotor.h"
#include "Cm_Camera.h"
#include "Scene.h"

#include "Renderer.h"

Engine::Entity* ent1;
Engine::Entity* ent2;
Engine::Entity* ent3;
Engine::Entity* camera;
void registerInputs() {
  // Todo: read these from file
  // using pushback rather than a={...} as the ps3 compiler is old and dumb
  std::vector<std::string> a;

  a.push_back("kb_space");
  a.push_back("gp_1_start");
  a.push_back("ms_l");
  Engine::Input::addMap("action1", a);
  a.clear();
  a.push_back("ms_r");
  Engine::Input::addMap("action2", a);
  a.clear();
}

void Game::init() {
  // TODO: allow code gnerated meshes to be stored in mesh storage
  /*
 Engine::Mesh torus;
 // load a Torus mesh from the torus generator
 torus.vertexData = CreateTorus(1, 5, 10, 10);
 torus.loadedMain = true;
 torus.numVerts = torus.vertexData.size();
 torus.strip = true;
 Engine::MeshLoader->loadOnGPU(&torus);
 */

  ent1 = new Engine::Entity();
  ent1->SetName("Cube Ent");
  ent1->setPosition(Vector3(0, 0, 0));
  ent1->AddComponent(new Engine::Components::CmMeshRenderer());
  ent1->getComponent<Engine::Components::CmMeshRenderer>()->setMesh(
      "models/cube.obj");
  // TODO: do this automagically
  Engine::ActiveScene->AddEntity(ent1);

  ent2 = new Engine::Entity();
  ent2->SetName("floor");
  ent2->setPosition(Vector3(0, 0, 0));
  ent2->setScale(Vector3(60, 1, 60));
  ent2->AddComponent(new Engine::Components::CmMeshRenderer());
  ent2->getComponent<Engine::Components::CmMeshRenderer>()->setMesh(
      "models/plane.obj");
  Engine::ActiveScene->AddEntity(ent2);

  ent3 = new Engine::Entity();
  ent3->SetName("Tex cube");
  ent3->setPosition(Vector3(5, 5, 5));
  ent3->AddComponent(new Engine::Components::CmMeshRenderer());
  Engine::Components::CmMeshRenderer* rc;
  rc = ent3->getComponent<Engine::Components::CmMeshRenderer>();
  rc->setMesh("models/cube.obj");
  rc->SetMaterial("flat");
  rc->SetMaterialTexture("coolimage.png");
  rc->SetMaterialTexture(0,"coolimageNormal.png");

  // TODO: do this automagically
  Engine::ActiveScene->AddEntity(ent3);

  camera = new Engine::Entity();
  camera->SetName("Camera");
  camera->setPosition(Vector3(0, 5.5, 0));
  // ent3->setPosition(Vector3(-70, 30, -70));
  // ent3->setPosition(Vector3(-30, 2.5, 0));
  camera->setRotation(Vector3(0.115247, 0.956242, 0.000000));
  // ent3->setRotation(Vector3(0.0f, 0.0f, 0));
  camera->AddComponent(new Engine::Components::CmFpsMotor());
  camera->AddComponent(new Engine::Components::CmCamera());
  camera->getComponent<Engine::Components::CmCamera>()->Activate();
  Engine::ActiveScene->AddEntity(camera);

  registerInputs();
}

void Game::update(double delta) {
  Engine::Renderer->DrawLine(Vector3(0, 0, 0), Vector3(0, 6, 0));
  Engine::Renderer->DrawCross(Vector3(0, 6, 0), 1.0f);
}

void Game::render() {}

void Game::shutdown() {
  delete ent1;
  delete ent2;
  delete camera;
}
