#include "../Utilities.h"
#include "OGL_Shader.h"
#include "glew\glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../FileIO.h"
namespace Engine {
namespace OGL {

void OGL_Shader::LoadBinaryShader(std::string name) {
  ASSERT_MSG((_shaderType),
             "_shaderType must be set before loading a shader file!");
  /*
  const char *buffer;
  int shaderCompiled = 0;
  int maxLength = 0;

  char *shaderSource = FileIO::LoadBinaryFile(name);
  ///already compiled shader? possible?

  _shaderID = glCreateShader(_shaderType);
  glShaderSource(_shaderID, 1, (const GLchar **)&shaderSource, 0);
  glCompileShader(_shaderID);

  glGetShaderiv(_shaderID,	GL_COMPILE_STATUS,	&shaderCompiled);

  if(shaderCompiled != GL_TRUE) {
  // Retrieve error status, store in error string
  glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

  // The maxLength includes the NULL character
  char * errorStr = new char[maxLength];
  glGetShaderInfoLog(_shaderID, maxLength, &maxLength, errorStr);
  printf("shader error: %s\n",errorStr);
  // Set shader ID to 0 to indicate failure
  _shaderID = 0;
  }
  */
}

void OGL_Shader::LoadSourceShader(std::string name) {
  name = "shaders/" + name;
  if (engineDebug){printf("Looking for shader: %s\n", name.c_str());}

  ASSERT_MSG((_shaderType),
             "_shaderType must be set before loading a shader file!");
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("An OGL error ahas occured: %u\n", err);
    HALT;
  }

  int shaderCompiled = 0;
  int maxLength = 0;

  char *shaderSource = FileIO::LoadTextFile(name);
  // COMPILE
  _shaderID = glCreateShader(_shaderType);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("An OGL error has occured: %u\n", err);
    HALT;
  }

  glShaderSource(_shaderID, 1, (const GLchar **)&shaderSource, 0);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("An OGL error ahas occured: %u\n", err);
    HALT;
  }

  glCompileShader(_shaderID);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("An OGL error ahas occured: %u\n", err);
    HALT;
  }

  glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &shaderCompiled);

  if (shaderCompiled != GL_TRUE) {
    // Retrieve error status, store in error string
    glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

    /* The maxLength includes the NULL character */
    char *errorStr = new char[maxLength];
    glGetShaderInfoLog(_shaderID, maxLength, &maxLength, errorStr);
    printf("shader error: %s\n", errorStr);
    // Set shader ID to 0 to indicate failure
    _shaderID = 0;
  }
 
  delete shaderSource;
}

//! Parses a shader stored in a char array and stores it into Program in an
//unitialised state
void OGL_Shader::loadCharArrayShader(const unsigned char *shaderCode){};

//! Initializes the shader program.
void OGL_Shader::initProgram(bool storeOnRSX){};

//! Set a named paramter value
void OGL_Shader::SetParameter(std::string name, float *data){};

//! Transposes a matrix into a 2d float[] and stores into a named paramter value
void OGL_Shader::SetParameterM(std::string name, Matrix4 &totranpose){};
}
}