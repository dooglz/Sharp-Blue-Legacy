#pragma once
#include "Component.h"

namespace Engine {
class Mesh;
namespace Components {

class CmMeshRenderer : public CComponent {
protected:
  Mesh* _mesh;

public:
  CmMeshRenderer();
  ~CmMeshRenderer();
  Mesh* getMesh();
  void setMesh(Mesh* msh);
  void setMesh(std::string meshname);
  void Render();
  bool culled;
};
}
}