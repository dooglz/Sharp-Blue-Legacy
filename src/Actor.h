#pragma once
#include "Utilities.h"		// asserts and commmon defines
#include "Mesh.h"
namespace Engine{
	class Actor{
		protected:
			bool _visible;
		private:

		public:

			stMesh* Mesh;
			vector3 position;
			vector3 rotation;
			vector3 scale;
			matrix4 getModelProjection();
		//	void render();
			bool isVisible();
			void setVisibility(const bool vis);
			void Render();
	};
}