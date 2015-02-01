#include "SDL_Event_Manager.h"
#include "../Utilities.h"
#include "sdl\SDL.h"
#include "SDL_Platform.h"
#include "../Input.h"
#include "../GameEngine.h"

#include <Rocket/Core.h>
#include "../LibRocketInterface.h"

namespace Engine {
namespace OGL {

void SDLEventManager::processEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
      case SDL_QUIT: {
        GameEngine::StopLoop();
        break;
      }

      // INPUT EVENTS --#
      case SDL_KEYDOWN: {
        Rcontext->ProcessKeyDown(CLibRocketInterface::TranslateKey(e.key.keysym.sym), 0);
        switch (e.key.keysym.sym) {
          // function keys
          case SDLK_SPACE: {
            Input::setValue("kb_space", 255);
            break;
          }
          case SDLK_ESCAPE: {
            GameEngine::StopLoop();
            break;
          }

          case SDLK_RETURN: {
            Input::setValue("kb_enter", 255);
            break;
          }
          // arrowkeys
          case SDLK_UP: {
            Input::setValue("kb_up", 255);
            break;
          }
          case SDLK_LEFT: {
            Input::setValue("kb_left", 255);
            break;
          }
          case SDLK_RIGHT: {
            Input::setValue("kb_right", 255);
            break;
          }
          case SDLK_DOWN: {
            Input::setValue("kb_down", 255);
            break;
          }
          // wasd
          case SDLK_w: {
            Input::setValue("kb_k_w", 255);
            break;
          }
          case SDLK_a: {
            Input::setValue("kb_k_a", 255);
            break;
          }
          case SDLK_s: {
            Input::setValue("kb_k_s", 255);
            break;
          }
          case SDLK_d: {
            Input::setValue("kb_k_d", 255);
            break;
          }
        }
        break;
      }
      case SDL_KEYUP: {
        switch (e.key.keysym.sym) {
          case SDLK_SPACE: {
            Input::setValue("kb_space", 0);
            break;
          }
          case SDLK_RETURN: {
            Input::setValue("kb_enter", 0);
            break;
          }
          case SDLK_UP: {
            Input::setValue("kb_up", 0);
            break;
          }
          case SDLK_LEFT: {
            Input::setValue("kb_left", 0);
            break;
          }
          case SDLK_RIGHT: {
            Input::setValue("kb_right", 0);
            break;
          }
          case SDLK_DOWN: {
            Input::setValue("kb_down", 0);
            break;
          }
          // wasd
          case SDLK_w: {
            Input::setValue("kb_k_w", 0);
            break;
          }
          case SDLK_a: {
            Input::setValue("kb_k_a", 0);
            break;
          }
          case SDLK_s: {
            Input::setValue("kb_k_s", 0);
            break;
          }
          case SDLK_d: {
            Input::setValue("kb_k_d", 0);
            break;
          }
        }
        break;
      }
     case SDL_MOUSEWHEEL: {
        Rcontext->ProcessMouseWheel(e.wheel.y, 0);
        break;
      }
      case SDL_MOUSEMOTION: {
        Rcontext->ProcessMouseMove(e.motion.x, e.motion.y, 0);
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        Rcontext->ProcessMouseButtonDown(CLibRocketInterface::TranslateMouseButton(e.button.button), 0);
        switch (e.button.button) {
          case SDL_BUTTON_LEFT: {
            Input::setValue("ms_l", 255);
            break;
          }
          case SDL_BUTTON_RIGHT: {
            Input::setValue("ms_r", 255);
            break;
          }
          case SDL_BUTTON_MIDDLE: {
            break;
          }
        }
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        Rcontext->ProcessMouseButtonUp(CLibRocketInterface::TranslateMouseButton(e.button.button), 0);
        switch (e.button.button) {
          case SDL_BUTTON_LEFT: {
            Input::setValue("ms_l", 0);
            break;
          }
          case SDL_BUTTON_RIGHT: {
            Input::setValue("ms_r", 0);
            break;
          }
          case SDL_BUTTON_MIDDLE: {
            break;
          }
        }
        break;
      }
      case SDL_JOYAXISMOTION: {
        break;
      }
      case SDL_JOYBALLMOTION: {
        break;
      }
      case SDL_JOYHATMOTION: {
        break;
      }
      case SDL_JOYBUTTONDOWN: {
        break;
      }
      case SDL_JOYBUTTONUP: {
        break;
      }
      default: { break; }
    }
    // End switch
  }
  // End While
}

void SDLEventManager::init() {
  printf("SDLEventManager Init \n");
  Input::addAxis("ms_l");
  Input::addAxis("ms_r");
  Input::addAxis("kb_up");
  Input::addAxis("kb_down");
  Input::addAxis("kb_left");
  Input::addAxis("kb_right");
  Input::addAxis("kb_space");
  Input::addAxis("kb_enter");
  // wasd
  Input::addAxis("kb_k_w");
  Input::addAxis("kb_k_a");
  Input::addAxis("kb_k_s");
  Input::addAxis("kb_k_d");
}
void SDLEventManager::Shutdown() {
  printf("SDLEventManager Shutdown \n");
  Input::WipeAll();
}
}
}