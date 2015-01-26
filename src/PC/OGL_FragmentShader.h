#pragma once
#include "OGL_Shader.h"
namespace Engine {
namespace OGL {
class OGL_FragmentShader : public OGL_Shader {

public:
  static OGL_FragmentShader* Load(const std::string &name);

  OGL_FragmentShader();
  //! Set a named paramter value
  // void SetParameter (CGparameter param, float * data );

  //! Reloads the instruction cache of the fragment shader
  void UpdateShaderVariables();
};
}
}