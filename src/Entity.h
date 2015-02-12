#pragma once
#include "Utilities.h"
#include <string>
#include <vector>
#include <typeinfo>
#include "Maths.h"
#include "Component.h"
namespace Engine {
  class Scene;
  class Entity {
  protected:
    bool _visible;
    std::string _name;
    bool _changed;
    Vector3 _position;
    Quaternion _rotation;
    Vector3 _scale;
    Matrix4 _transform;
    void CalculateTransform();
    std::vector<CComponent*> _components;
    Scene* _scene;
  public:
    //returns the scene that this entity belongs to.
    Scene* GetScene();
    //Called from addEntity in scene.
    void SetScene(Scene* scn);

    Entity();
    ~Entity();

    Vector3 getScale();
    Vector3 getPosition();
    Quaternion getRotation();
    Matrix4 getTranform();

    // TODO void setTransform(const Matrix4 m4);
    void setScale(const Vector3& v3);
    void setPosition(const Vector3& v3);
    void setRotation(const Vector3& v3);
    void setRotation(const Quaternion& q);
    bool isVisible();
    void setVisibility(const bool b);
    void SetName(std::string const& name);
    std::string GetName() const;
    virtual void Update(const double delta);
    virtual void Render();

    void AddComponent(CComponent* const c);
    void RemoveComponent(CComponent* const c);
    std::vector<CComponent*> GetComponents(std::string const& name) const;

    template <typename T>
    T* const getComponent() {
      std::vector<CComponent*>::iterator it;
      for (std::vector<CComponent*>::iterator it = _components.begin();
        it != _components.end(); ++it) {
        // printf("Checking %s against %s \n", typeid(**it).name(),
        // typeid(T).name());
        if (&typeid(**it) == &typeid(T)) {
          return static_cast<T*>(*it);
        }
      }
      return NULL;
    }
  };
}