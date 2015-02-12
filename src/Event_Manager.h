#pragma once
#include "Utilities.h"
namespace Engine {
class CEventManager {

protected:
public:
  virtual void processEvents(){};
  virtual void init(){};
  virtual void Shutdown(){};
  virtual ~CEventManager(){};
};

// global public reference to the EventManager
extern CEventManager* EventManager;
}