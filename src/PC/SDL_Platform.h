#pragma once
#include "../Platform.h"
#include <sdl/SDL.h>
namespace Rocket {
  namespace Core {
    class Context;
  }
}
namespace Engine {

namespace SDL {
class SDL_Platform : public CPlatform {
private:
protected:
  static bool _running;
  static SDL_Window* _window;
  static SDL_GLContext _gContext;
  unsigned char _glverMaj;
  unsigned char _glverMin;

  void InitDisplay(const unsigned short width, const unsigned short height);

public:
  SDL_Platform();
  void Init(const unsigned short width, const unsigned short height);
  double GetTime();
  void Shutdown();

  static void CheckGL();
  static void CheckSDL();

  static SDL_Window* GetWindow() { return _window; };
  // void SaveFile(const std::string& name);
};
}
extern Rocket::Core::Context* Rcontext;
}