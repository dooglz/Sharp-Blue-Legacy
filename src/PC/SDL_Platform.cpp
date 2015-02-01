#include <stdio.h>
#include <cstring>
#include <iostream>

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger/Debugger.h>

#include "SDL_Platform.h"
#include "../Utilities.h"
#include "OGL_Renderer.h"
#include "SDL_Meshloader.h"
#include "SDL_Event_Manager.h"
#include <glew/glew.h>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>

#include "../LibRocketInterface.h"
//#include <sdl\SDL_opengl.h>
#define DEFAULT_HEIGHT 720
#define DEFAULT_WIDTH 1280

Rocket::Core::Context* Engine::Rcontext = NULL;

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

void GetFirstNMessages(GLuint numMsgs)
{
  GLint maxMsgLen = 0;
  glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

  std::vector<GLchar> msgData(numMsgs * maxMsgLen);
  std::vector<GLenum> sources(numMsgs);
  std::vector<GLenum> types(numMsgs);
  std::vector<GLenum> severities(numMsgs);
  std::vector<GLuint> ids(numMsgs);
  std::vector<GLsizei> lengths(numMsgs);

  GLuint numFound = glGetDebugMessageLog(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);
  if (numFound == 0){
    numFound = glGetDebugMessageLogAMD(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &lengths[0], &msgData[0]);
  }
  if (numFound == 0){
    numFound = glGetDebugMessageLogARB(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);
  }

  sources.resize(numFound);
  types.resize(numFound);
  severities.resize(numFound);
  ids.resize(numFound);
  lengths.resize(numFound);

  std::vector<std::string> messages;
  messages.reserve(numFound);

  std::vector<GLchar>::iterator currPos = msgData.begin();
  for (size_t msg = 0; msg < lengths.size(); ++msg)
  {
    messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
    currPos = currPos + lengths[msg];
  }
}


void SDL_Platform::CheckGL() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("An OGL error has occured: %i\n", err);
   // GetFirstNMessages(10);
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
  InitUI();


}

void SDL_Platform::InitDisplay(const unsigned short width,
                               const unsigned short height) {
  _window = SDL_CreateWindow(
      "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH,
      DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  ASSERT(_window != NULL);

  // context flags
  // IMPORTANT! -------------------------------------------------------
  //TODO: make this depend on debug build.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
  //-------------------------------------------------------------------
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // Create context
  _gContext = SDL_GL_CreateContext(_window);
  CheckSDL();
  CheckGL();
  ASSERT(_gContext != NULL);

  // init Glew
  glewExperimental = GL_TRUE;
  SDL_assert(glewInit() == GLEW_OK);
  glGetError(); // Experimental init throws junk errors, Ignore.

  GlewInfo();
  CheckGL();
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

void SDL_Platform::InitUI() {
  printf("Librocket version: %s\n\n", Rocket::Core::GetVersion());

  CLibRocketInterface* uii = new CLibRocketInterface();
  Rocket::Core::SetSystemInterface(uii);
  CLibRocketRenderInterface* uir = new CLibRocketRenderInterface();
  Rocket::Core::SetRenderInterface(uir);

  Rocket::Core::Initialise();
  Rocket::Controls::Initialise();

  Rcontext = Rocket::Core::CreateContext(
      "default", Rocket::Core::Vector2i(_screenWidth, _screenHeight));

  Rocket::Debugger::Initialise(Rcontext);

  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-BoldItalic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-Bold.otf");

  Rocket::Core::ElementDocument* document =
      Rcontext->LoadDocument("ui/tutorial.rml");
  if (document != NULL) {
    document->Show();
   // document->RemoveReference();
  } else {
    ASSERT(false);
  }
  Rocket::Debugger::SetVisible(true);
}
}
}
