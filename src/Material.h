#pragma once
#include "Resource.h"
namespace Engine{
  Material mat_Default = {"Default","basic","basic",NULL,NULL,NULL,2};
}
/*
#pragma once
#include <string>
namespace Engine {

struct Material {
  const std::string materialName;
  const std::string vsName;
  const std::string fsName;
  void* program;
  void* VS;
  void* FS;
  const unsigned int ParametersCount;
  const std::string* ParametersNames[];
};


const std::string params1[] = { "Vertex Position", "Vertex Colour" };
const std::string params2[] = { "Vertex Position", "Texture Coords" };
static const Material random = { "random", "basic", "basic", NULL, NULL, NULL, 2, params1 };
static const Material flate = { "flat", "textured", "textured", NULL, NULL, NULL, 2, params2 };



struct flat : Material{

};


}

*/