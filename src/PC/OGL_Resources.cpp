#include "../OGL_Resources.h"
#include "../Resource.h"
#include "../Storage.h"
#include "../Material.h"
#include "OGL_ShaderProgram.h"
//
#include "glew/glew.h"
#include "SDL_platform.h"
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
  T->EngineTexture = Storage<OGL::OGL_Texture>::Get(name);
  return T;
}
Texture::~Texture() {
  // delete (static_cast<OGL::OGL_ShaderProgram*>(EngineTexture));
}

namespace OGL {
OGL_Texture::~OGL_Texture() {}
OGL_Texture::OGL_Texture() 
{
  
  glGenTextures(1, &TextureID);
  SDL::SDL_Platform::CheckGL();
 // glBindTexture(GL_TEXTURE_2D, TextureID);
//  SDL::SDL_Platform::CheckGL();
  //TODO: Load Texture file from disk
 // imageData = ...
 // glTexImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 0, 0, imageData);


}
OGL_Texture* OGL_Texture::Load(const std::string& name) { return NULL;}
void OGL_Texture::LoadOnGpu() {}
}
}