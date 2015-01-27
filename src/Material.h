#pragma once
#include "Resource.h"
namespace Engine{
  enum Materials { Default, Textured };
  struct MaterialData{
    const std::string materialName;
    const std::string vsName;
    const std::string fsName;
    const unsigned int ParametersCount;
    const unsigned int TexturesCount;
  };
  const MaterialData MaterialStats[] = { { "Default", "basic", "basic", 0, 0 }, { "Textured", "basic", "basic", 1, 0 } };
}
