#include <stdio.h>
#include <cstring>
#include <iostream>

#include "LibRocketInterface.h"

#include "SDL_Platform.h"
#include "../Utilities.h"
#include "OGL_Renderer.h"
#include "SDL_Meshloader.h"
#include "SDL_Event_Manager.h"
#include <glew/glew.h>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>

#define DEFAULT_HEIGHT 720
#define DEFAULT_WIDTH 1280

void SDLVersionInfo() {
  SDL_version compiled;
  SDL_version linked;

  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  printf("Compiled against SDL version %d.%d.%d ...\n", compiled.major,
         compiled.minor, compiled.patch);
  printf("linking against SDL version %d.%d.%d.\n", linked.major, linked.minor,
         linked.patch);

  SDL_version compile_version;
  const SDL_version* link_version = IMG_Linked_Version();
  SDL_IMAGE_VERSION(&compile_version);
  printf("compiled with SDL_image version: %d.%d.%d\n", compile_version.major,
         compile_version.minor, compile_version.patch);
  printf("running with SDL_image version: %d.%d.%d\n", link_version->major,
         link_version->minor, link_version->patch);
}

void GlewInfo() {
  printf("----------------------------------------------------------------\n");
  printf("OpenGL Info\n");
  printf("Glew Version: %s\n", glewGetString(GLEW_VERSION));
  printf("     Version: %s\n", glGetString(GL_VERSION));
  printf("      Vendor: %s\n", glGetString(GL_VENDOR));
  printf("    Renderer: %s\n", glGetString(GL_RENDERER));
  printf("     Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("----------------------------------------------------------------\n");
}

SDL_assert_state CustomAssertionHandler(const SDL_assert_data* data,
                                        void* userdata) {
  Engine::SDL::SDL_Platform::CheckGL();
  Engine::SDL::SDL_Platform::CheckSDL();
  SDL_AssertionHandler defaultHandler = SDL_GetDefaultAssertionHandler();
  return defaultHandler(data, userdata);
}

void __stdcall DebugCallbackAMD(GLuint id, GLenum category, GLenum severity,
  GLsizei length, const GLchar* message,
  GLvoid* userParam) {
  printf("\nAn OGL AMD error has occured: %s\n", message);
}

void __stdcall DebugCallbackARB(GLenum source, GLenum type, GLuint id,
  GLenum severity, GLsizei length,
  const GLchar* message, GLvoid* userParam) {
  printf("\nAn OGL ARB error has occured: %s\n", message);
}

void __stdcall printOutKhrDebugMessage(GLenum source, GLenum type, GLuint id,
  GLenum severity, GLsizei length,
  const GLchar* message,
  const void* userParam) {
  printf("\nAn OGL KHR error has occured: %s\n", message);
}

namespace Engine {
unsigned short CPlatform::_screenWidth;
unsigned short CPlatform::_screenHeight;

namespace SDL {

double SDL_Platform::GetTime() {
  return ((double)(SDL_GetPerformanceCounter()) /
          ((double)SDL_GetPerformanceFrequency()));
}

SDL_Window* SDL_Platform::_window;
SDL_GLContext SDL_Platform::_gContext;

void SDL_Platform::CheckGL() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("An OGL error has occured: %i\n", err);
  }
}

void SDL_Platform::CheckSDL() {
  const char* err = SDL_GetError();
  if (strlen(err) != 0) {
    printf("SDL error: %s\n", err);
    SDL_ClearError();
  }
}

SDL_Platform::SDL_Platform() {
  printf("SDL Platform Loading\n");
  SDLVersionInfo();
  // Initialize SDL
  SDL_SetAssertionHandler(CustomAssertionHandler, NULL);
  ASSERT((SDL_Init(SDL_INIT_VIDEO) >= 0));
}

void SDL_Platform::Init(const unsigned short width,
                        const unsigned short height) {

  EventManager = new OGL::SDLEventManager();
  EventManager->init();

  InitDisplay(width, height);
  Renderer = new OGL::COGL_Renderer();
  Renderer->Init();
  IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG);

  UserInterface = new CLibrocket();
  UserInterface->Init();
}

void SDL_Platform::InitDisplay(const unsigned short width,
                               const unsigned short height) {
  _window = SDL_CreateWindow(
      "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH,
      DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  ASSERT(_window != NULL);

  //Temp context
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // Create context
  _gContext = SDL_GL_CreateContext(_window);
  CheckSDL();
  CheckGL();
  ASSERT(_gContext != NULL);
  // init Glew
  glewExperimental = GL_TRUE;
  ASSERT(glewInit() == GLEW_OK);
  glGetError(); // Experimental init throws junk errors, Ignore.

  GlewInfo();
  CheckGL();

  if (!GLEW_VERSION_4_3)
  {
    printf("\n\nThis engine requires a minimum of opengl 4.3\n");
    ASSERT(false);
    //TODO exit
  }

  SDL_GL_DeleteContext(_gContext);
  CheckSDL();


  // context flags
  // IMPORTANT! -------------------------------------------------------
  //TODO: make this depend on debug build.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
  //-------------------------------------------------------------------
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  // Create context
  _gContext = SDL_GL_CreateContext(_window);
  CheckSDL();
  CheckGL();
  ASSERT(_gContext != NULL);

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
  CheckGL();
  if (GLEW_ARB_debug_output) {
    printf("Supporting Arb OGL debug  output\n");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallbackARB((GLDEBUGPROCARB)DebugCallbackARB, 0);
    CheckGL();
  }

  if (GLEW_AMD_debug_output) {
    printf("Supporting AMD OGL debug output\n");
    glDebugMessageCallbackAMD((GLDEBUGPROCAMD)DebugCallbackAMD, 0);
    CheckGL();
  }

  if (GLEW_KHR_debug) {
    printf("Supporting KHR OGL debug output\n");
    glDebugMessageCallback((GLDEBUGPROC)printOutKhrDebugMessage, 0);
    CheckGL();
  }

  _screenHeight = DEFAULT_HEIGHT;
  _screenWidth = DEFAULT_WIDTH;
  printf("Display Resolution: %i x %i\n", _screenWidth, _screenHeight);
  // Vsync
  ASSERT(SDL_GL_SetSwapInterval(1) >= 0);

}

void SDL_Platform::Shutdown() {
  UserInterface->Shutdown();
  delete UserInterface;
  UserInterface = NULL;

  Renderer->Shutdown();
  delete Renderer;
  Renderer = NULL;

  EventManager->Shutdown();
  delete EventManager;
  EventManager = NULL;

  IMG_Quit();

  SDL_GL_DeleteContext(_gContext);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
/*
void SDL_Platform::SaveFile(const std::string& name) {
    int x = 0;
    int y = 0;
    int w = _screenWidth;
    int h = _screenHeight;

    //_gContext->
    unsigned char* pixels = new unsigned char[w * h * 4];  // 4 bytes for RGBA
    glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    SDL_Surface* surf =
        SDL_CreateRGBSurfaceFrom(pixels, w, h, 8 * 4, w * 4, 0, 0, 0, 0);
    SDL_SaveBMP(surf, name.c_str());

    SDL_FreeSurface(surf);
    delete[] pixels;
}
*/

}
}
