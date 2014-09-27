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
			Vector3 position;
			Vector3 rotation;
			Vector3 scale;
			Matrix4 getModelProjection();
		//	void render();
			bool isVisible();
			void setVisibility(const bool vis);
			void Render();
	};
}