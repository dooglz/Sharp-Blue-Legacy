#include "OGL_ShaderProgram.h"
#include "glew\glew.h"
#include <sstream>
#include "SDL_platform.h"
#include "../Storage.h"

namespace Engine {
namespace OGL {

OGL_ShaderProgram::~OGL_ShaderProgram(){

  printf("shader program shurdown \n");
}


OGL_ShaderProgram::OGL_ShaderProgram() { 
  _ID = glCreateProgram();
  VS = NULL;
  FS = NULL;

 }

void OGL_ShaderProgram::link() {
  glLinkProgram(_ID);

  // check for link errors
  GLint programSuccess = GL_TRUE;
  glGetProgramiv(_ID, GL_LINK_STATUS, &programSuccess);
  if (programSuccess != GL_TRUE) {
    printf("Error linking program %u!\n", _ID);
    char* errorStr = new char[256];
    glGetProgramInfoLog(_ID, 256, 0, errorStr);
    printf("program link error: %s\n", errorStr);
    HALT
  }
  if (engineDebug){printf("linking program %u\n", _ID);}
  // check for general errors
  // checkerr();
}

void OGL_ShaderProgram::attachShader(OGL_Shader* shader) {
  glAttachShader(_ID, shader->getID());
  // checkerr();
}

GLuint OGL_ShaderProgram::getID() { return _ID; }

OGL_ShaderProgram* OGL_ShaderProgram::Load(const std::string& name) {
  std::string vsName;
  std::string fsName;

  {
    // Todo: move to a helper class
    std::stringstream ss(name);
    std::string item;
    bool b = 0;
    while (std::getline(ss, item, '-')) {
      if (!b) {
        vsName = item + ".vert";
        b = 1;
      } else {
        fsName = item + ".frag";
      }
    }
  }

  OGL::OGL_VertexShader* VS = Storage<OGL::OGL_VertexShader>::Get(vsName);
  OGL::OGL_FragmentShader* FS = Storage<OGL::OGL_FragmentShader>::Get(fsName);
  OGL_ShaderProgram* pgrm = new OGL_ShaderProgram();
  if (VS == NULL || FS == NULL) {
    printf("Trying to link some null shaders!\n");
    return NULL;
  }

  pgrm->attachShader(VS);
  SDL::SDL_Platform::CheckGL();
  pgrm->attachShader(FS);
  SDL::SDL_Platform::CheckGL();
  pgrm->FS = FS;
  pgrm->VS = VS;
  // Link program
  pgrm->link();
  SDL::SDL_Platform::CheckGL();

  return pgrm;
}


}
}