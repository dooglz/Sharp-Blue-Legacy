#pragma once
namespace Engine {
class CEventManager {

protected:
public:
  virtual void processEvents(){};
  virtual void init(){};
  virtual ~CEventManager(){};
};

// global public reference to the EventManager
extern CEventManager* EventManager;
}