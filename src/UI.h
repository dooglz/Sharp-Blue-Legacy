#pragma once
#include "Utilities.h"
#include <string>
#include <vector>

class EventBouncer;

namespace Engine {

class UIDocument {
protected:
  std::vector<EventBouncer*> _bouncers;
public:
  UIDocument(){};
  ~UIDocument();
  void* internalPointer;
  void SetContent(const std::string& elementID, const std::string& content);
  std::string GetContent(const std::string& elementID);
  void AddEventListener(const std::string& elementID, const std::string& eventType, void(*ListenerFunction)(const std::string& elementID));
  void RemoveEventListener(const std::string& elementID, const std::string& eventType, void(*ListenerFunction)(const std::string& elementID));
  void RemoveAllListeners();
};

class UICanvas {
protected:
  std::vector<UIDocument*> _documents;
public:
  UICanvas(unsigned int posX, unsigned int posY, unsigned int sizeX,
           unsigned int sizeY);
  void* internalPointer;
  const unsigned int posX, posY, sizeX, sizeY;
  UIDocument* LoadDocument(const std::string& name);
  void Unload();
  ~UICanvas();
};

class CUserInterface {
protected:

public:
  virtual ~CUserInterface() = 0;
  virtual void Update(const double delta) = 0;
  virtual void Render() = 0;
  virtual void Init() = 0;
  virtual void Shutdown() = 0;
  virtual UICanvas* NewCanvas(const unsigned int posX, const unsigned int posY,
                              const unsigned int sizeX,
                              const unsigned int sizeY,
                              const std::string& name) = 0;
};

extern CUserInterface* UserInterface;
}