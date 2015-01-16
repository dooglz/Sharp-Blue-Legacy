#pragma once
#include <string>

namespace Engine {
	class Entity;
	class CComponent {
	protected:
		// Parent Entity
		Entity* Ent;
		bool _active;
		const std::string _token;

	public:
		CComponent(const std::string token);
		~CComponent();
		virtual void Update(double delta){};
		virtual void Render(){};
		bool Active();
		void setActive(bool b);
		void SetParent(Entity* p);
		Entity* GetParent();
		std::string GetToken();
	};
}