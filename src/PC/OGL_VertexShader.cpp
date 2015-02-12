#include "OGL_VertexShader.h"
#include "glew\glew.h"
#include "SDL_Platform.h"
namespace Engine {
namespace OGL {
OGL_VertexShader::OGL_VertexShader() {
  _shaderType = GL_VERTEX_SHADER;
  VERTEX_COLOUR_INDEX = 0;
  VERTEX_POSITION_INDEX = 0;
}

OGL_VertexShader *OGL_VertexShader::Load(const std::string &name) {
  // Create Fragment shader
  OGL_VertexShader *VS = new OGL_VertexShader();
  VS->LoadSourceShader(name);
  SDL::SDL_Platform::CheckGL();
  return VS;
}
}
}