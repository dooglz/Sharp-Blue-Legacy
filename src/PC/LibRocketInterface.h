#pragma once
#include "../Utilities.h"
#include <vector>
#include "sdl\SDL.h"
#include "../ui.h"
#include <Rocket/Core.h>
#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"

namespace Engine {

class CLibrocket : public CUserInterface {
private:
  std::vector<Rocket::Core::Context*> contexts;
  CLibRocketInterface* _uii;
  CLibRocketRenderInterface* _uir;
public:
  void CLibrocket::ProcessMouseWheel(int delta);
  void CLibrocket::ProcessMouseMove(int x, int y);
  void CLibrocket::ProcessMouseButtonDown(Uint8 sdlbutton);
  void CLibrocket::ProcessMouseButtonUp(Uint8 sdlbutton);
  void ProcessKeyDown(SDL_Keycode sdlkey);

  CLibrocket();
  ~CLibrocket();

  void Update(const double delta);
  void Render();
  void Init();
  void Shutdown();

  UICanvas* NewCanvas(const unsigned int posX, const unsigned int posY,
                      const unsigned int sizeX, const unsigned int sizeY,
                      const std::string& name);
  void RemoveCanvas(UICanvas* canvas);
};
}