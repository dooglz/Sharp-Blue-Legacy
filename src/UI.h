#pragma once
#include <string>
#include "Utilities.h" // asserts and commmon defines

namespace Engine {
  class UICanvas{
  public:
    UICanvas(unsigned int posX, unsigned int posY, unsigned int sizeX, unsigned int sizeY);
    void* internalPointer;
    const unsigned int posX, posY, sizeX, sizeY;
    void LoadDocument(const std::string &name);
    void Unload();
  };

  class CUserInterface {
  public:
    virtual void Update(const double delta) = 0;
    virtual void Render() = 0;
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual UICanvas* NewCanvas(const unsigned int posX, const unsigned int posY, const unsigned int sizeX, const unsigned int sizeY, const std::string& name) = 0;
    //Will delete the Uicanvas element    
    virtual void RemoveCanvas(UICanvas* canvas) = 0;
};

  extern CUserInterface* UserInterface;
}