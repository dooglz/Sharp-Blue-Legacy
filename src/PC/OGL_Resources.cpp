#include "OGL_Resources.h"
#include "../Resource.h"
#include "../Storage.h"
#include "../Material.h"
#include "OGL_ShaderProgram.h"
//
#include "glew/glew.h"
#include "SDL_platform.h"
#include <sdl/SDL_image.h>

namespace Engine {

Material::Material(const std::string& name, const std::string& vs,
                   const std::string& fs, unsigned int Params,
                   unsigned int Textures, const bool NeedsNormals,
                   const bool NeedsColours, const bool NeedsUVs)
    : materialName(name),
      vsName(vs),
      fsName(fs),
      ParametersCount(Params),
      TexturesCount(Textures),
      NeedsNormals(NeedsNormals),
      NeedsColours(NeedsColours),
      NeedsUVs(NeedsUVs) {}

Material* Material::Load(const std::string& name) {
  // TODO: what the hell are you doing son?!
  bool found = 0;
  unsigned int index = 0;
  for (MaterialData md : MaterialStats) // How is this even possible?
  {
    if (md.materialName == name) {
      found = true;
      break;
    }
    index++;
  }
  ASSERT(found);
  Material* m = new Material(
      name, MaterialStats[index].vsName, MaterialStats[index].fsName,
      MaterialStats[index].ParametersCount, MaterialStats[index].TexturesCount,
      MaterialStats[index].NeedsNormals, MaterialStats[index].NeedsColours,
      MaterialStats[index].NeedsUVs);
  m->EngineMaterial =
      Storage<OGL::OGL_ShaderProgram>::Get(m->vsName + "-" + m->fsName);
  return m;
}
Material::~Material() {
  // shader programs are stored in storage will be autodeleted on shutdown
  // delete (static_cast<OGL::OGL_ShaderProgram*>(EngineMaterial));
}

Texture* Texture::Load(const std::string& name) {

Texture* T = new Texture();

  T->EngineTexture = Storage<OGL::OGL_Texture>::Get(name);
  return T;
}
Texture::~Texture() {
  // delete (static_cast<OGL::OGL_ShaderProgram*>(EngineTexture));
}

namespace OGL {
OGL_Texture::~OGL_Texture() {
  SDL_FreeSurface(imageData);
  // delete imageData;
  imageData = NULL;
}
OGL_Texture::OGL_Texture() {
  imageData = NULL;
  TextureID = -1;
}

OGL_Texture* OGL_Texture::Load(const std::string& name) {
  SDL::SDL_Platform::CheckGL();
  printf("\nLoading texture: %s\n\n", name.c_str());

  SDL_Surface* surface = IMG_Load(name.c_str());
  SDL::SDL_Platform::CheckSDL();

  OGL_Texture* oglt = new OGL_Texture();
  // Load image from file into a surface
  oglt->imageData = surface;
  ASSERT(oglt->imageData != NULL);

  // http://content.gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL

  GLenum texture_format;
  GLenum internalFormat;

  // Check that the image's width is a power of 2
  if ((surface->w & (surface->w - 1)) != 0) {
    printf("warning: image.bmp's width is not a power of 2\n");
  }

  // Also check if the height is a power of 2
  if ((surface->h & (surface->h - 1)) != 0) {
    printf("warning: image.bmp's height is not a power of 2\n");
  }

  // get the number of channels in the SDL surface
  GLint nOfColors = surface->format->BytesPerPixel;
  if (nOfColors == 4) // contains an alpha channel
  {
    internalFormat = GL_RGBA;
    if (surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGBA;
    else
      texture_format = GL_BGRA;
  } else if (nOfColors == 3) // no alpha channel
  {
    internalFormat = GL_RGB;
    if (surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGB;
    else
      texture_format = GL_BGR;
  }
  else if (nOfColors == 1) //8bit image
  {
    //but does it have alpha?
      texture_format = GL_RGBA2;
      internalFormat = GL_RGB;
  }
  else {
    printf("warning: the image is not truecolor..  this will probably break\n");
    ASSERT(false);
  }

  SDL::SDL_Platform::CheckGL();
  // Have OpenGL generate a texture object handle for us
  glGenTextures(1, &oglt->TextureID);
  SDL::SDL_Platform::CheckGL();
  ASSERT(oglt->TextureID != -1);

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, oglt->TextureID);
  SDL::SDL_Platform::CheckGL();

  // Set the texture's stretching properties
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  SDL::SDL_Platform::CheckGL();

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
  SDL::SDL_Platform::CheckGL();
  return oglt;
}

void OGL_Texture::LoadOnGpu() {}
}
}