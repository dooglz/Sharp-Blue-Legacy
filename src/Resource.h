#pragma once
#include <string>
#include "Maths.h"
#include "Utilities.h"
#include <vector>

namespace Engine {
class FragmentShader;
class ShaderProgram;
class VertexShader;
namespace OGL {
class OGL_ShaderProgram;
}

enum Material_Parameter_type { MAT4, VEC2, VEC3, TEX };

struct Mesh {
//TODO: move outta headder
  /* Mesh() {
    loadedMain = 0;
    loadedLocal = 0;
    numVerts = 0;
    hasUvs = 0;
    hasNormals = 0;
    strip = 0;
    fan = 0;
    line = 0;
#if defined(_WINDOWS_)
    gVBO = 0;
    gVAO = 0;
    gUVBO = 0;
    gNORMBO = 0;
    gCOLOURBO = 0;
    gIBO = 0;
#endif
  }
  */
  bool loadedMain;
  bool loadedLocal;
  int numVerts;
  bool hasUvs;
  bool hasNormals;
  bool strip;
  bool fan;
  bool line;
#if defined(_WINDOWS_)
  unsigned int gVBO;
  unsigned int gVAO;
  unsigned int gUVBO;
  unsigned int gNORMBO;
  unsigned int gCOLOURBO;
  unsigned int gIBO;
#elif defined(_PS3_)
  // Pointer to the vertex buffer in local memory
  stVertex *vertexBuffer;
  unsigned int vertexBufferOffset;
#endif
  // mesh data in main memory
  std::vector<Vector3> vertexData;
  std::vector<unsigned int> colours;
  std::vector<Vector2> uvs;
  std::vector<Vector3> normals;
  static Mesh *Load(const std::string &name);
  void LoadOnGpu();
};

struct Texture {
  ~Texture();
  void *EngineTexture; // OGL_Texture*
  static Texture *Load(const std::string &name);
  void LoadOnGpu();
};

struct Material {
  Material(const std::string &name, const std::string &vs,
           const std::string &fs, const unsigned int Params,
           const unsigned int Textures, const bool NeedsNormals,
           const bool NeedsColours, const bool NeedsUVs);
  ~Material();
  void *EngineMaterial; // most likely a OGL_ShaderProgram*
  static Material *Load(const std::string &name);
  const std::string materialName;
  const std::string vsName;
  const std::string fsName;
  const unsigned int ParametersCount;
  const unsigned int TexturesCount;
  const bool NeedsNormals;
  const bool NeedsColours;
  const bool NeedsUVs;
};

struct RenderObject {
  ~RenderObject(){
    // Todo: URGENT
  };
  void *EngineRenderObject;
  Mesh *mesh;
  Material *material;
  Texture **textures;
  void **Params;
};
}

static void swapMeshColorEndianmode(Engine::Mesh *m) {
  /*CG shaders work in bigendian, which means that the stvertex.rgba
  will be read as (abgr), whereas glsl reads it as (rgba)*/
  for (int i = 0; i < (m->numVerts); ++i) {
    unsigned int r = m->colours[i];
    // Unsigned 16 bit conversion:
    // m->vertexData[i].rgba = ((r>>8) | (r<<8));

    // Unsigned 32-bit conversion:
    m->colours[i] = ((r >> 24) & 0xff) | ((r << 8) & 0xff0000) |
                    ((r >> 8) & 0xff00) | ((r << 24) & 0xff000000);
  }
}