#pragma once
#include "../Event_Manager.h"
namespace Engine {
namespace OGL {
class SDLEventManager : public CEventManager {

protected:
public:
  void processEvents();
  void init();
};
}
}