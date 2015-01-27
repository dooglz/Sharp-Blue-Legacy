#include "../OGL_Resources.h"
#include "../Resource.h"
#include "../Storage.h"
#include "../Material.h"
#include "OGL_ShaderProgram.h"
//
#include "glew/glew.h"
#include "SDL_platform.h"
namespace Engine {

Material::Material(const std::string& name, const std::string& vs,
                   const std::string& fs, unsigned int Params,
                   unsigned int Textures)
    : materialName(name),
      vsName(vs),
      fsName(fs),
      ParametersCount(Params),
      TexturesCount(Textures) {}

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
      MaterialStats[index].TexturesCount, MaterialStats[index].ParametersCount);
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

  printf("\nLoading texture: %s\n\n", name.c_str());
  // 'Warm Up' the texture loader, lolwut
  {
    //  SDL_Surface* lolut = SDL_LoadBMP("textures/NoiseA512.bmp");
    //  SDL::SDL_Platform::CheckSDL();
    // SDL_FreeSurface(temp);
  }

  SDL_Surface* surface = SDL_LoadBMP(name.c_str());

  OGL_Texture* oglt = new OGL_Texture();

  glEnable(GL_TEXTURE_2D);

  // Load image from file into a surface
  oglt->imageData = surf;
  SDL::SDL_Platform::CheckSDL();
  ASSERT(oglt->imageData != NULL);
//http://content.gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL







  GLenum texture_format;

  // Check that the image's width is a power of 2
  if ((surface->w & (surface->w - 1)) != 0) {
    printf("warning: image.bmp's width is not a power of 2\n");
  }

  // Also check if the height is a power of 2
  if ((surface->h & (surface->h - 1)) != 0) {
    printf("warning: image.bmp's height is not a power of 2\n");
  }

  // get the number of channels in the SDL surface
  GLint  nOfColors = surface->format->BytesPerPixel;
  if (nOfColors == 4)     // contains an alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGBA;
    else
      texture_format = GL_BGRA;
  }
  else if (nOfColors == 3)     // no alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGB;
    else
      texture_format = GL_BGR;
  }
  else {
    printf("warning: the image is not truecolor..  this will probably break\n");
    // this error should not go unhandled
  }

  // Have OpenGL generate a texture object handle for us
  glGenTextures(1, &texture);

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set the texture's stretching properties
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Edit the texture object's image data using the information SDL_Surface gives us
  glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
    texture_format, GL_UNSIGNED_BYTE, surface->pixels);














  /*
  SDL_Surface* newSurface = SDL_CreateRGBSurface(0, 512, 512, 24, 0xff000000,
0x00ff0000, 0x0000ff00, 0);
  SDL_BlitSurface(oglt->imageData, 0, newSurface, 0); // Blit onto a purely RGB
Surface

  SDL_Surface* newSurface16 = SDL_CreateRGBSurface(0, 512, 512, 16, 0xff0000,
0x00ff00, 0x0000ff, 0);
  SDL_BlitSurface(oglt->imageData, 0, newSurface16, 0); // Blit onto a purely
RGB Surface


//if 16bit
// 
  SDL_Surface* rgb16 = SDL_LoadBMP("textures/test16.bmp");
  SDL_Surface* rgb16a = SDL_LoadBMP("textures/test16A.bmp");

  SDL_Surface* rgb24 = SDL_LoadBMP("textures/test24.bmp");
  SDL_Surface* rgb24a = SDL_LoadBMP("textures/test24A.bmp");


  SDL_Surface* rgb32 = SDL_LoadBMP("textures/test32.bmp");
  SDL_Surface* rgb32a = SDL_LoadBMP("textures/test32A.bmp");

  SDL_Surface* rgb16toRGB444 = SDL_ConvertSurfaceFormat(rgb16,
SDL_PIXELFORMAT_RGB555, 0);

*/
  SDL_Surface* rgb16toRGB444;

  if (surf->format->BitsPerPixel == 15){
    printf("\nTexture is 16bit\n");
    if (surf->format->Rloss || surf->format->Bloss || surf->format->Gloss)
    {
      printf("\nTexture is an odd format\n");
      rgb16toRGB444 = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGB555, 0);

    }

  }
  //SDL_FreeSurface(surf);

  SDL::SDL_Platform::CheckSDL();
  // setup ogl texture
  glGenTextures(1, &oglt->TextureID);
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1)​;

  SDL::SDL_Platform::CheckGL();
  ASSERT(oglt->TextureID != -1);

  glBindTexture(GL_TEXTURE_2D, oglt->TextureID);
  SDL::SDL_Platform::CheckGL();
  GLint formart = GL_RGB;

  // send surface to ogl
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);
  SDL::SDL_Platform::CheckGL();
  return oglt;
}

void OGL_Texture::LoadOnGpu() {}
}
}