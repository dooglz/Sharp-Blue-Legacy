#include "OGL_FragmentShader.h"
#include "glew\glew.h"
#include "SDL_Platform.h"
namespace Engine {
namespace OGL {
OGL_FragmentShader::OGL_FragmentShader() { _shaderType = GL_FRAGMENT_SHADER; }

OGL_FragmentShader *OGL_FragmentShader::Load(const std::string &name) {

  // Create Fragment shader
  OGL_FragmentShader *FS = new OGL_FragmentShader();
  FS->LoadSourceShader(name);
  SDL::SDL_Platform::CheckGL();
  return FS;
}
}
}