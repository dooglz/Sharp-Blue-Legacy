#pragma once
#include "../Utilities.h"
#include "OGL_FragmentShader.h"
#include "OGL_VertexShader.h"

namespace Engine {
namespace OGL {
class OGL_ShaderProgram {

public:
  OGL_ShaderProgram();
  ~OGL_ShaderProgram();
  static OGL_ShaderProgram *Load(const std::string &name);

  OGL_VertexShader* VS;
  OGL_FragmentShader* FS;

  std::string name;

  void link();

  void attachShader(OGL_Shader* shader);

  unsigned int getID();

protected:
  unsigned int _ID;
};
}
}