#include "Component.h"

namespace Engine {
  CComponent::CComponent(const std::string& token) : _token(token) {
    _active = true;
    Ent = 0;
  }
  CComponent::~CComponent(){
    printf("base component destructor\n");
};
  bool CComponent::Active() { return _active; }
  void CComponent::setActive(bool b) { _active = b; }
  void CComponent::SetParent(Entity* p) { Ent = p; };
  std::string CComponent::GetToken() { return _token; }
  Entity* CComponent::GetParent() { return Ent; }
}