#pragma once
#include "Component.h"

namespace Engine {
 struct Mesh;
struct RenderObject;
namespace Components {

class CmMeshRenderer : public CComponent {
protected:
  RenderObject* _ro;
public:
  CmMeshRenderer();
  ~CmMeshRenderer();

  Mesh* getMesh();
  void setMesh(Mesh* msh);
  void setMesh(std::string meshname);

  void SetMaterial(std::string materialName);
  void* getMaterial();
  void SetMaterialTexture(std::string textureName);
  void SetMaterialTexture(unsigned int i, std::string textureName);

  void Render();
  bool culled;
};
}
}