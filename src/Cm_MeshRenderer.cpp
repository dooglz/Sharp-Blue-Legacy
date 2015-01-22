#include "Cm_MeshRenderer.h"
#include "Renderer.h"
#include "Entity.h"
#include "MeshLoader.h"
#include "Resource.h"
#include "MAterial.h"
#include <iostream>
#include <string>

namespace Engine {
namespace Components {
CmMeshRenderer::CmMeshRenderer() : CComponent("MeshRenderer") {
  _ro = new RenderObject();
  _ro->material = &mat_Default;
}

CmMeshRenderer::~CmMeshRenderer() { delete _ro; }

Mesh* CmMeshRenderer::getMesh() { return _ro->mesh; }

void CmMeshRenderer::setMesh(Mesh* msh) { _ro->mesh = msh; }

void CmMeshRenderer::setMesh(std::string meshname) {
  // TODO: Have an actual meshloader platform
  _ro->mesh = MeshLoader->getMesh(meshname);
  MeshLoader->loadOnGPU(_ro->mesh);
  // TODO: Have an actual shader selection platform
  //_mesh->program = OGL::COGL_Renderer::GetDefaultShaderProgram();
  // null shaders get defaulted in renderer
}

void CmMeshRenderer::SetMaterial(std::string materialName) {
  // assume flat
}
void* CmMeshRenderer::getMaterial() { return _ro->material; }

void CmMeshRenderer::SetMaterialTexture(std::string textureName) {
  SetMaterialTexture(0, textureName);
}

void CmMeshRenderer::SetMaterialTexture(unsigned int i, std::string textureName)
{
  Texture* texture; //= getTexture(textureName);
  //Loader->loadOnGPU(texture);
 // _ro->textures[i] = texture;

}

void CmMeshRenderer::Render() {
  ASSERT(_ro->mesh != NULL);
  Renderer->RenderMesh(_ro, Ent->getTranform());
}
}
}