#include "OGL_ShaderProgram.h"
#include "glew\glew.h"
namespace Engine {
namespace OGL {

OGL_ShaderProgram::OGL_ShaderProgram() { _ID = glCreateProgram(); }

void OGL_ShaderProgram::link() {
  glLinkProgram(_ID);

  // check for link errors
  GLint programSuccess = GL_TRUE;
  glGetProgramiv(_ID, GL_LINK_STATUS, &programSuccess);
  if (programSuccess != GL_TRUE) {
    printf("Error linking program %d!\n", _ID);
    char* errorStr = new char[256];
    glGetProgramInfoLog(_ID, 256, 0, errorStr);
    printf("program link error: %s\n", errorStr);
    HALT
  }

  // check for general errors
  // checkerr();
}

void OGL_ShaderProgram::attachShader(OGL_Shader* shader) {
  glAttachShader(_ID, shader->getID());
  // checkerr();
}

GLuint OGL_ShaderProgram::getID() { return _ID; }
}
}