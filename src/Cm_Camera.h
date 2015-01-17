#pragma once
#include "Component.h"

namespace Engine {
namespace Components {
class Mesh;
class CmCamera : public CComponent {
protected:
  Mesh* _mesh;
  bool _active;

public:
  CmCamera();
  ~CmCamera();

  void Activate();
  void Deactivate();
  bool IsActive();

  // void Render();
  // void Update(double delta);
};
}
}