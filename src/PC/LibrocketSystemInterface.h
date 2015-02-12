#pragma once
#include "../Utilities.h"
#include <Rocket/Core.h>
#include <Rocket/Core/SystemInterface.h>
namespace Engine{
class CLibRocketInterface : public Rocket::Core::SystemInterface {
public:
  CLibRocketInterface();
  ~CLibRocketInterface();
  virtual float GetElapsedTime();
  static Rocket::Core::Input::KeyIdentifier CLibRocketInterface::TranslateKey(
    signed __int32 sdlkey);
  static int TranslateMouseButton(unsigned __int8 button);
  bool LogMessage(Rocket::Core::Log::Type type,
                  const Rocket::Core::String& message);
};
}