#include "Entity.h"
#include <string>
#include <algorithm>
#include "Maths.h"
#include "Component.h"

namespace Engine {

  //Returns the scene that this entity belongs to.
  Scene* Entity::GetScene(){ return _scene; }
  //Called from addEntity in scene.
  void Entity::SetScene(Scene* scn){_scene = scn; }

  Entity::Entity() {
    _visible = true;
    _scale = Vector3(1.0f, 1.0f, 1.0f);
    _position = Vector3(0.0f, 0.0f, 0.0f);
    _rotation = Quaternion::identity();
    CalculateTransform();
  }

  Entity::~Entity() {
    printf("Entity destructor\n");
    for (std::vector<CComponent *>::iterator it = _components.begin();
      it != _components.end(); ++it) {
      delete (*it);
    }
  }

  Vector3 Entity::getScale() { return _scale; }
  Vector3 Entity::getPosition() { return _position; }
  Quaternion Entity::getRotation() { return _rotation; }

  void Entity::setScale(const Vector3 &v3) {
    if (IsIdentical(v3, _scale)) {
      return;
    }
    _changed = true;
    _scale = v3;
  }

  void Entity::setPosition(const Vector3 &v3) {
    if (IsIdentical(v3, _position)) {
      return;
    }
    _changed = true;
    _position = v3;
  }

  void Entity::setRotation(const Vector3 &v3) { setRotation(EulerToQuat(v3)); }

  void Entity::setRotation(const Quaternion &q) {
    if (IsIdentical(q, _rotation)) {
      return;
    }
    _changed = true;
    _rotation = q;
  }

  std::string Entity::GetName() const { return _name; }

  void Entity::SetName(std::string const &name) { _name = name; }

  Matrix4 Entity::getTranform() {
    if (!_changed) {
      return _transform;
    }
    else {
      CalculateTransform();
      return _transform;
    }
  }

  void Entity::CalculateTransform() {
    Matrix4 scl = Scale(_scale);
    Matrix4 rot = QuatToMatrix(_rotation);
    Matrix4 trn = Translation(_position);
    //_transform =  (scl * rot * trn);
    //_transform = (trn * rot * scl);
   _transform = (trn * scl * rot);
   // _transform = (rot * scl * scl);
    //_transform = (trn * scl);
    _changed = false;
  }

  bool Entity::isVisible() { return _visible; }
  void Entity::setVisibility(const bool b) { _visible = b; }

  void Entity::Update(const double delta) {
    for (std::vector<CComponent *>::iterator it = _components.begin();
      it != _components.end(); ++it) {
      (*it)->Update(delta);
    }
  };

  void Entity::Render() {
    for (std::vector<CComponent *>::iterator it = _components.begin();
      it != _components.end(); ++it) {
      (*it)->Render();
    }
  };

  void Entity::AddComponent(CComponent *const c) {
    _components.push_back(c);
    c->SetParent(this);
  }

  void Entity::RemoveComponent(CComponent *const c) {
    std::vector<CComponent *>::iterator position =
      std::find(_components.begin(), _components.end(), c);
    if (position != _components.end()) {
      _components.erase(position);
    }
  }

  std::vector<CComponent *> Entity::GetComponents(std::string const &name) const {
    std::vector<CComponent *> c;
    if (_components.size() < 1) {
      return c;
    }
    std::vector<CComponent *>::iterator it;
    for (std::vector<CComponent *>::const_iterator it = _components.begin();
      it != _components.end(); ++it) {
      if ((*it)->GetToken() == name) {
        c.push_back((*it));
      }
    }
    return c;
  }
}