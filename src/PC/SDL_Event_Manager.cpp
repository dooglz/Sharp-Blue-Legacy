#include "SDL_Event_Manager.h"
#include "../Utilities.h"
#include "sdl\SDL.h"
#include "SDL_Platform.h"
#include "../Input.h"
#include "../GameEngine.h"

#include "LibRocketInterface.h"

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
      ((CLibrocket *)UserInterface)->ProcessKeyDown(e.key.keysym.sym);
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
      case SDLK_q: {
        Input::setValue("kb_k_q", 255);
        break;
      }
      case SDLK_e: {
        Input::setValue("kb_k_e", 255);
        break;
      }
      case SDLK_r: {
        Input::setValue("kb_k_r", 255);
        break;
      }
      case SDLK_t: {
        Input::setValue("kb_k_t", 255);
        break;
      }
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

      case SDLK_q: {
        Input::setValue("kb_k_q", 0);
        break;
      }
      case SDLK_e: {
        Input::setValue("kb_k_e", 0);
        break;
      }
      case SDLK_r: {
        Input::setValue("kb_k_r", 0);
        break;
      }
      case SDLK_t: {
        Input::setValue("kb_k_t", 0);
        break;
      }
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
      ((CLibrocket *)UserInterface)->ProcessMouseWheel(e.wheel.y);
      break;
    }
    case SDL_MOUSEMOTION: {
      Input::setValue("ms_x", e.motion.x);
      Input::setValue("ms_y", e.motion.y);
      ((CLibrocket *)UserInterface)->ProcessMouseMove(e.motion.x, e.motion.y);
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      ((CLibrocket *)UserInterface)->ProcessMouseButtonDown(e.button.button);
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
      ((CLibrocket *)UserInterface)->ProcessMouseButtonUp(e.button.button);
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

  Input::addAxis("kb_up");
  Input::addAxis("kb_down");
  Input::addAxis("kb_left");
  Input::addAxis("kb_right");
  Input::addAxis("kb_space");
  Input::addAxis("kb_enter");
  // wasd
  Input::addAxis("kb_k_q");
  Input::addAxis("kb_k_e");
  Input::addAxis("kb_k_r");
  Input::addAxis("kb_k_t");

  Input::addAxis("kb_k_w");
  Input::addAxis("kb_k_a");
  Input::addAxis("kb_k_s");
  Input::addAxis("kb_k_d");
  //mouse pos
  Input::addAxis("ms_x");
  Input::addAxis("ms_y");
  //mouse buttons
  Input::addAxis("ms_l");
  Input::addAxis("ms_r");
}
void SDLEventManager::Shutdown() {
  printf("SDLEventManager Shutdown \n");
  Input::WipeAll();
}
}
}