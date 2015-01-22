#pragma once
#include "Component.h"

namespace Engine {
struct Mesh;
struct Texture;
struct Material;
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
  void setMesh(const std::string& meshname);

  //void SetMaterial(const std::string& materialName);
  void SetMaterial(Material* mat);
  void* getMaterial();

  void SetMaterialTexture(unsigned int i, const std::string& textureName);
  void SetMaterialTexture(unsigned int i, Texture* tex);

  void Render();
  bool culled;
};
}
}