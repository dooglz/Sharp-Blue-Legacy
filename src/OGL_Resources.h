#pragma once
#include <string>
#include "PC/SDL_platform.h"

namespace Engine {
namespace OGL {
struct OGL_Texture {
  ~OGL_Texture();
  OGL_Texture();
  static OGL_Texture* Load(const std::string& name);
  void LoadOnGpu();
  unsigned int TextureID;
  SDL_Surface* imageData;
};
}
}