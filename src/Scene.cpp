#include "Entity.h"
#include "Scene.h"
#include "Renderer.h"
#include "Cm_Camera.h"

extern Matrix4 oviewMatrix;

namespace Engine {
	Scene* ActiveScene = NULL;

	int Scene::Population() const { return _ents.size(); }

	void Scene::AddEntity(Entity* const e) {
		// TODO: check to see if already have entitiy
		_ents.push_back(e);
	}

	bool Scene::RemoveEntity(Entity* const e) { return false; }
	const std::vector<Entity*> Scene::GetList() { return _ents; }

	void Scene::Update(const double delta) {
		if (_ents.size() < 1) {
			return;
		}
		for (std::vector<Entity*>::iterator it = _ents.begin(); it != _ents.end();
			++it) {
			(*it)->Update(delta);
		}
	}

	void Scene::Render() {
		static unsigned int i = 0;
		if (_ents.size() < 1) {
			return;
		}
		Matrix4 viewMat;
		if (_activeCamera == NULL) {
			printf("No active Camera in Scene!\n");
			//Renderer->SetViewMatrix(Matrix4(1.0f));
		}
		else {
			Quaternion cq = _activeCamera->GetParent()->getRotation();
			Vector3 pos = _activeCamera->GetParent()->getPosition();

			Vector3 up = Normalize(GetUpVector(cq));
			Vector3 forwards = Normalize(GetForwardVector(cq));
			Vector3 right = Normalize(GetRightVector(cq));

			// pos.setY(-pos.getY());
#if defined(_PC_)
			viewMat = glm::lookAt(pos, pos + forwards, up);
#elif defined(_PS3_)
			// pos.setY(-pos.getY());
			float px = pos.getX();
			float py = pos.getY();
			float pz = pos.getZ();
			//
			float fx = forwards.getX();
			float fy = forwards.getY();
			float fz = forwards.getZ();
			//
			float ux = up.getX();
			float uy = up.getY();
			float uz = up.getZ();
			//
			pos = Vector3(px, py, pz);
			forwards = Vector3(fx, fy, fz);
			up = Vector3(ux, uy, uz);

			// pos = (-30.0f, 10.0f, -30.0f);
			viewMat = lookat(Vector3(-100.0f, 60.0f, -100.0f),
				Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
			// up = Cross(right, forwards);
			// viewMat = lookat(pos, pos + forwards, up);
#endif

			if (i % 100 == 0) {
				printf("\n\n\n\n\n\n");
				print(pos);
				print(up);
				print(forwards);
				print(viewMat);
			}
			++i;

			//Renderer->SetViewMatrix(viewMat);
		}

		for (std::vector<Entity*>::iterator it = _ents.begin(); it != _ents.end();
			++it) {
			(*it)->Render();
		}
	}

	void Scene::Shutdown() {}

	CmCamera* Scene::GetActiveCamera() const { return _activeCamera; }

	void Scene::SetActiveCamera(CmCamera* const c) {
		if (_activeCamera != NULL) {
			_activeCamera->Deactivate();
		}
		_activeCamera = c;
	}

	void Scene::report() const {
		printf("Scene report -- Ents:%i\n", _ents.size());
		for (std::vector<Entity*>::const_iterator it = _ents.begin();
			it != _ents.end(); ++it) {
			printf("%s - (%i,%i,%i)\n", (*it)->GetName(), (*it)->getPosition().getX(),
				(*it)->getPosition().getY(), (*it)->getPosition().getZ());
		}
	}
}