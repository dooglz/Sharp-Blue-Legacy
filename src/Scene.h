#pragma once
#include "Entity.h"
#include <vector>

namespace Engine {
	//	class Entity;
	class CmCamera;
	class Scene {
	protected:
		std::vector<Entity*> _ents;
		CmCamera* _activeCamera;

	public:
		int Population() const;
		void AddEntity(Entity* const e);
		bool RemoveEntity(Entity* const e);
		const std::vector<Entity*> GetList();
		void Update(const double delta);
		void Render();
		void Shutdown();
		CmCamera* GetActiveCamera() const;
		void SetActiveCamera(CmCamera* const c);
		void report() const;
	};

	// global public reference to the renderer
	extern Scene* ActiveScene;
}