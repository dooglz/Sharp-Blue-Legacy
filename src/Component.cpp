#include "Component.h"

namespace Engine {
	CComponent::CComponent(const std::string token) : _token(token) {
		_active = true;
	}
	CComponent::~CComponent(){};
	bool CComponent::Active() { return _active; }
	void CComponent::setActive(bool b) { _active = b; }
	void CComponent::SetParent(Entity* p) { Ent = p; };
	std::string CComponent::GetToken() { return _token; }
	Entity* CComponent::GetParent() { return Ent; }
}