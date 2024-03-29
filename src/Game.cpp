// THIS GAME FILE IS FOR DEBUGINNG THE ENGINE, PROJECT USING THE ENGINE WILL
// HAVE THEIR OWN GAME FILE
#include "Entity.h"
#include "Maths.h"
#include "Game.h"
#include "Input.h"

#include "GameEngine.h"
#include "Cm_MeshRenderer.h"
#include "Cm_FpsMotor.h"
#include "Cm_Camera.h"
#include "Scene.h"
#include "UI.h"
#include "Renderer.h"

#include "Material.h"

Engine::Entity* ent1;
Engine::Entity* ent2;
Engine::Entity* ent3;
Engine::Entity* ent4;
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
  a.push_back("ms_x");
  Engine::Input::addMap("pointerX", a);
  a.clear();
  a.push_back("ms_y");
  Engine::Input::addMap("pointerY", a);
  a.clear();
}

void EventListener(const std::string& elementID) {
  printf("%s G1 has been pressed\n", elementID.c_str());
}

void EventListener2(const std::string& elementID) {
  printf("%s G2 has been pressed\n", elementID.c_str());
}
static Engine::UICanvas* canvas;
static Engine::Scene* defaultScene;
void Game::init() {
  // Loading scene
  defaultScene = new Engine::Scene();
  Engine::GameEngine::SetActiveScene(defaultScene);
  canvas = Engine::UserInterface->NewCanvas(0, 0, 1280, 720, "Default");
  Engine::UIDocument* doc = canvas->LoadDocument("ui/ui.rml");
  doc->SetContent("button1", "We set this element with code!");
  doc->AddEventListener("button1", "click", EventListener);
  doc->AddEventListener("button2", "click", EventListener);
  doc->AddEventListener("button2", "click", EventListener2);
  //doc->AddEventListener("button3", "click", EventListener2);
 // doc->AddEventListener("button4", "click", EventListener);
 // doc->AddEventListener("button4", "click", EventListener2);

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
  ent2->getComponent<Engine::Components::CmMeshRenderer>()->SetMaterial(
      Engine::Materials::Textured);
  ent2->getComponent<Engine::Components::CmMeshRenderer>()->SetMaterialTexture(
      0, "textures/placeholder2.jpg");

  ent3 = new Engine::Entity();
  ent3->SetName("Tex cube");
  ent3->setPosition(Vector3(5, 5, 5));
  ent3->AddComponent(new Engine::Components::CmMeshRenderer());
  Engine::Components::CmMeshRenderer* rc;
  rc = ent3->getComponent<Engine::Components::CmMeshRenderer>();
  rc->setMesh("models/cube.obj");
  rc->SetMaterial(Engine::Materials::Textured);
  rc->SetMaterialTexture(0, "textures/test24.bmp");

  ent4 = new Engine::Entity();
  ent4->SetName("title");
  ent4->setPosition(Vector3(0, 3, 0));
  ent4->setScale(Vector3(1, 1, 1));
  ent4->setRotation(Vector3(0.5f * M_PI, M_PI, 0.5f * -M_PI));
  ent4->AddComponent(new Engine::Components::CmMeshRenderer());
  ent4->getComponent<Engine::Components::CmMeshRenderer>()->setMesh(
      "models/plane.obj");
  Engine::ActiveScene->AddEntity(ent4);
  ent4->getComponent<Engine::Components::CmMeshRenderer>()->SetMaterial(
      Engine::Materials::Textured);
  ent4->getComponent<Engine::Components::CmMeshRenderer>()->SetMaterialTexture(
      0, "textures/Title.png");

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

  Engine::ActiveScene->AddEntity(camera);
  Engine::ActiveScene->SetActiveCamera(
      camera->getComponent<Engine::Components::CmCamera>());

  registerInputs();
}

static float a = 0.01f;
void Game::update(double delta) {
  // printf("%f\n", QuatToEuler(ent4->getRotation()).x);
  ent4->setRotation(QuatToEuler(ent4->getRotation()) + Vector3(a, 0, 0));
  Engine::Renderer->DrawLine(Vector3(0, 0, 0), Vector3(0, 6, 0));
  Engine::Renderer->DrawCross(Vector3(0, 6, 0), 1.0f);
  //Engine::GameEngine::StopLoop();

  if (Engine::Input::getMapData("action2") > 128) {
    printf("action pressed 2\n");
    printf("mouse pos: %u, %u\n", Engine::Input::getMapData("pointerX"), Engine::Input::getMapData("pointerY"));
  }

}

void Game::render() {}

void Game::shutdown() {

  Engine::ActiveScene->RemoveAllEntities();
  delete defaultScene;
  ent1 = 0;
  ent2 = 0;
  ent3 = 0;
  ent4 = 0;
  camera = 0;
  camera = 0;

  if (canvas != NULL) {
    delete canvas;
    canvas = 0;
  }
}
