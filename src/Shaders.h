#pragma once
#include "Utilities.h"
#include "Maths.h"
#include <string>

namespace Engine {

//All these are are placehodlers for load functions, 
// the type of object returned is actually an platform type.
class FragmentShader{
public:
  void* actualProgram;
  static FragmentShader* Load(std::string name){return NULL;};
};

class VertexShader{
public:
  void* actualProgram;
  static VertexShader* Load(std::string name){ return NULL; };
};
/*
class ShaderProgram {
  void link();
  FragmentShader* FS;
  VertexShader* VS;
};*/
}