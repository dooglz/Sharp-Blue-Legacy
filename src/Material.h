#pragma once
#include "Utilities.h"
#include "Resource.h"
namespace Engine {
enum Materials { Default, Coloured, Textured };
struct MaterialData {
  const std::string materialName;
  const std::string vsName;
  const std::string fsName;
  const unsigned int ParametersCount;
  const unsigned int TexturesCount;
  bool NeedsNormals;
  bool NeedsColours;
  bool NeedsUVs;
};
const MaterialData MaterialStats[] = {
  { "Default", "basic", "basic", 1, 0, false, false, false },
  { "Coloured", "basic", "basic", 0, 0, false, true, false },
  { "Textured", "textured", "textured", 0, 1, false, false, true }
};
}
