#include "Resource.h"
#include "Storage.h"
#include "PC/OGL_ShaderProgram.h"

namespace Engine {

void Material::Load() {

  // the program will do dthis
  // OGL::OGL_VertexShader* VS = Storage<OGL::OGL_VertexShader>::Get(vsName);
  // OGL::OGL_FragmentShader* FS =
  // Storage<OGL::OGL_FragmentShader>::Get(fsName);
  EngineMaterial = Storage<OGL::OGL_ShaderProgram>::Get(vsName + "-" + fsName);

  // TODO: mkae program a get from resource
  // printf("Linking shader - %s with %s\n", vsName.c_str(), fsName.c_str());
  // program = Renderer->MakeProgram(FS, VS);
}

}