#pragma once
#include "Utilities.h"
namespace Engine {
class CPlatform {
protected:
  //! Output screen Width, in pixels
  static unsigned short _screenWidth;

  //! Output screen Height, in pixels
  static unsigned short _screenHeight;

  //! Output screen ratio, width/height
  static float _screenRatio;

public:
  CPlatform(){};
  virtual ~CPlatform(){};
  virtual void Init(const unsigned short width,
                    const unsigned short height) = 0;
  virtual void Shutdown() = 0;
  virtual double GetTime() = 0;
  static unsigned short GetSreenWidth() { return _screenWidth; }
  static unsigned short GetSreenHeight() { return _screenHeight; }
  // static float getScreenRatio();
};
extern CPlatform* Platform;
}
