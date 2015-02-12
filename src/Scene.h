#pragma once
#include "Utilities.h"
#include <vector>

namespace Engine {
  class Entity;

namespace Components {
class CmCamera;
}
class Scene {
protected:
  std::vector<Entity*> _ents;
  Components::CmCamera* _activeCamera;

public:
  Scene();
  int Population() const;
  void AddEntity(Entity* const e);
  bool RemoveEntity(Entity* const e);
  //Calls Delete on evey entity in the scne, clears the scenelist
  void RemoveAllEntities();

  const std::vector<Entity*> GetList();
  Components::CmCamera* GetActiveCamera() const;
  void SetActiveCamera(Components::CmCamera* const c);

  virtual void Update(const double delta);
  virtual void report() const;

  //Called when this scene becomes the active scene
  virtual void Load(){};

  //Called when this scene is no longer the active scene
  virtual void Unload(){};

  virtual void Render();

  virtual void Shutdown();
};

// global public reference to the active scene
extern Scene* ActiveScene;
}