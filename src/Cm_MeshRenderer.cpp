#include "Cm_MeshRenderer.h"
#include "Renderer.h"
#include "Entity.h"
#include "MeshLoader.h"
#include "Mesh.h"

#include <iostream>
#include <string>

namespace Engine {
namespace Components {
CmMeshRenderer::CmMeshRenderer() : CComponent("MeshRenderer") {}

CmMeshRenderer::~CmMeshRenderer() {}

Mesh* CmMeshRenderer::getMesh() { return _mesh; }

void CmMeshRenderer::setMesh(Mesh* msh) { _mesh = msh; }

void CmMeshRenderer::setMesh(std::string meshname) {
  // TODO: Have an actual meshloader platform
  _mesh = MeshLoader->getMesh(meshname);
  MeshLoader->loadOnGPU(_mesh);
  // TODO: Have an actual shader selection platform
  //_mesh->program = OGL::COGL_Renderer::GetDefaultShaderProgram();
  // null shaders get defaulted in renderer
}

void CmMeshRenderer::Render() {
  ASSERT(_mesh != NULL);

  Renderer->RenderMesh(_mesh, Ent->getTranform());
}
}
}