#include "../Resource.h"
#include "../Storage.h"
#include "../Material.h"
#include "OGL_ShaderProgram.h"

namespace Engine {

Material::Material(const std::string& name, const std::string& vs,
                   const std::string& fs, unsigned int Params,
                   unsigned int Textures)
    : materialName(name),
      vsName(vs),
      fsName(fs),
      ParametersCount(Params),
      TexturesCount(Textures) {}

Material* Material::Load(const std::string& name) {
  // TODO: what the hell are you doing son?!
  bool found = 0;
  unsigned int index = 0;
  for (MaterialData md : MaterialStats) // How is this even possible?
  {
    if (md.materialName == name) {
      found = true;
      break;
    }
    index++;
  }
  ASSERT(found);
  Material* m = new Material(
      name, MaterialStats[index].vsName, MaterialStats[index].fsName,
      MaterialStats[index].TexturesCount, MaterialStats[index].ParametersCount);
  m->EngineMaterial =
      Storage<OGL::OGL_ShaderProgram>::Get(m->vsName + "-" + m->fsName);
  return m;
}
Material::~Material() {
  // shader programs are stored in storage will be autodeleted on shutdown
  // delete (static_cast<OGL::OGL_ShaderProgram*>(EngineMaterial));
}

Texture* Texture::Load(const std::string& name) {
  Texture* T = new Texture();
  // T->EngineTexture = Storage<OGL::OGL_ShaderProgram>::Get(name);
  return T;
}
Texture::~Texture() {
  // delete (static_cast<OGL::OGL_ShaderProgram*>(EngineTexture));
}
}