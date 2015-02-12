#pragma once
#include "Utilities.h"
#include "Component.h"

namespace Engine {
namespace Components {
class Mesh;
class CmCamera : public CComponent {
protected:

public:
  CmCamera();
  ~CmCamera();

  bool IsActive();

  // void Render();
  // void Update(double delta);
};
}
}