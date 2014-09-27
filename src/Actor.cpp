#include "Actor.h"
#include "GameEngine.h"	
#include "Renderer.h"

namespace Engine{
	bool Actor::isVisible(){ return _visible; }
	void Actor::setVisibility(const bool vis){ _visible = vis; }

	Matrix4 Actor::getModelProjection()
	{
		//calculate Model projection
		Matrix4 scl = M4::scale(scale);
		Matrix4 rot = M4::rotation(rotation);
		Matrix4 trn = M4::translation(position);
		return (scl * rot * trn);
		//return (trn * rot * scl);
	}
	void Actor::Render()
	{
		Engine::GameEngine::Renderer->renderMesh(Mesh, 
			Engine::GameEngine::Renderer->projMatrix *  
			Engine::GameEngine::Renderer->viewMatrix *
			getModelProjection());
	}

}