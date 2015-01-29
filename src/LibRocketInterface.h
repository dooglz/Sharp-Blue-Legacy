#pragma once
#include <Rocket/Core.h>
#include <Rocket/Core/SystemInterface.h>

namespace Engine {
  class CLibRocketInterface : public Rocket::Core::SystemInterface {
public:
  CLibRocketInterface();
  ~CLibRocketInterface();
  virtual float GetElapsedTime();
};
}