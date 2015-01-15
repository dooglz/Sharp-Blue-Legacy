#include "Base_Platform.h"
namespace Engine {
unsigned short Platform::_screenWidth;

//! Output screen Height, in pixels
unsigned short Platform::_screenHeight;

//! Output screen ratio, width/height
float Platform::_screenRatio;

unsigned short Platform::getScreenWidth() { return _screenWidth; }

unsigned short Platform::getScreenHeight() { return _screenHeight; }

float Platform::getScreenRatio() { return _screenRatio; }
}