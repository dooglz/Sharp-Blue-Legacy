#include "../Resource.h"
#include "../Storage.h"
#include "OGL_ShaderProgram.h"

namespace Engine {

void Material::Load() {
  EngineMaterial = Storage<OGL::OGL_ShaderProgram>::Get(vsName + "-" + fsName);
}
Material::~Material() {
  delete (static_cast<OGL::OGL_ShaderProgram*>(EngineMaterial));
}
}