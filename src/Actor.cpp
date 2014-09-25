#include "Actor.h"
namespace Engine{
	bool Actor::isVisible(){ return _visible; }
	void Actor::setVisibility(const bool vis){ _visible = vis; }

	matrix4 Actor::getModelProjection()
	{
		//calculate Model projection
		matrix4 scl = M4::scale(scale);
		matrix4 rot = M4::rotation(rotation);
		matrix4 trn = M4::translation(position);
		//return (scl * rot * trn);
		return (trn * rot * scl);
	}

}