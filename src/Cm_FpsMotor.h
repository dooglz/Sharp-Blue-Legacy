#pragma once
#include "Component.h"
namespace Engine {
namespace Components {
class CmFpsMotor : public CComponent {
protected:
  void registerInputs();

public:
  void Update(double delta);
  CmFpsMotor();
};
}
}