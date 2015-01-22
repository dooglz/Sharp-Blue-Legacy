#pragma once
#include <string>
#include "Maths.h"
#include "Utilities.h"
#include <vector>

namespace Engine {

namespace OGL{
  class OGL_ShaderProgram;
}

enum Material_Parameter_type {MAT4, VEC2, VEC3, TEX};

struct Mesh{
  bool loadedMain;
  bool loadedLocal;
  int numVerts;
  bool strip;
  bool fan;
  bool line;
#if defined(_WINDOWS_)
  unsigned int gVBO;
  unsigned int gVAO;
  unsigned int gIBO;
#elif defined(_PS3_)
  // Pointer to the vertex buffer in local memory
  stVertex* vertexBuffer;
  unsigned int vertexBufferOffset;
#endif
  // mesh data in main memory
  std::vector<stVertex> vertexData;
  static Mesh* Load(const std::string& name);
  void LoadOnGpu();
};

struct Texture{
  int lol;
  static Texture* Load(const std::string& name){return NULL;}
  void LoadOnGpu();
};

struct Material {
  const std::string materialName;
  const std::string vsName;
  const std::string fsName;
  OGL::OGL_ShaderProgram* program; //TODO: abstract
  void* VS;
  void* FS;
  const unsigned int ParametersCount;
  const unsigned int TexturesCount;
  //static Material* Load(const std::string& name){ return NULL; }
};

struct RenderObject {
  Mesh* mesh;
  Material* material;
  Texture** textures;
  void* otherParams;
};
}


static void swapMeshColorEndianmode(Engine::Mesh* m) {
  /*CG shaders work in bigendian, which means that the stvertex.rgba
  will be read as (abgr), whereas glsl reads it as (rgba)*/
  for (int i = 0; i < (m->numVerts); ++i) {
    unsigned int r = m->vertexData[i].rgba;
    // Unsigned 16 bit conversion:
    // m->vertexData[i].rgba = ((r>>8) | (r<<8));

    // Unsigned 32-bit conversion:
    m->vertexData[i].rgba = ((r >> 24) & 0xff) | ((r << 8) & 0xff0000) |
      ((r >> 8) & 0xff00) | ((r << 24) & 0xff000000);
  }
}