#include "Cm_MeshRenderer.h"
#include "Renderer.h"
#include "Entity.h"
#include "Resource.h"
#include "Storage.h"
#include "Material.h"
#include <iostream>
#include <string>

namespace Engine {
namespace Components {
CmMeshRenderer::CmMeshRenderer() : CComponent("MeshRenderer") {
  //_ro = new RenderObject();
  _ro = new RenderObject();
  _ro->textures = NULL;
  _ro->material = Materials::mat_Default;
}

CmMeshRenderer::~CmMeshRenderer() { delete _ro; }

Mesh* CmMeshRenderer::getMesh() { return _ro->mesh; }

void CmMeshRenderer::setMesh(Mesh* msh) {
  _ro->mesh = msh;
  _ro->mesh->LoadOnGpu();
}

void CmMeshRenderer::setMesh(const std::string& meshname) {
  setMesh(Storage<Mesh>::Get(meshname));
  // null shaders get defaulted in renderer
}

void CmMeshRenderer::SetMaterial(Material* mat) {
  mat->Load();
  _ro->material = mat;
  delete _ro->textures;
  delete _ro->Params;
  _ro->textures = new Texture* [_ro->material->TexturesCount];
  _ro->Params = new void* [_ro->material->ParametersCount];
}

void* CmMeshRenderer::getMaterial() { return _ro->material; }

void CmMeshRenderer::SetMaterialTexture(unsigned int i, Texture* tex) {
  // Loader->loadOnGPU(texture);
  if (i + 1 > _ro->material->TexturesCount) {
    return;
  }
  _ro->textures[i] = tex;
}

void CmMeshRenderer::SetMaterialTexture(unsigned int i,
                                        const std::string& textureName) {
  SetMaterialTexture(i, Storage<Texture>::Get(textureName));
}

void CmMeshRenderer::SetMaterialParameter(unsigned int i, void* param) {
  if (i + 1 > _ro->material->ParametersCount) {
    return;
  }
  _ro->Params[i] = param;
}

void CmMeshRenderer::Render() {
  ASSERT(_ro->mesh != NULL);
  Renderer->RenderMesh(_ro, Ent->getTranform());
}
}
}