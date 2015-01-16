#pragma once
#include "../MeshLoader.h"

namespace Engine {
	namespace SDL {
		class CSDL_Meshloader : public CMeshloader {
		protected:
		private:
		public:
			CSDL_Meshloader();
			Mesh* openOBJFile(const std::string& filename);
			void loadOnGPU(Mesh* msh);
			~CSDL_Meshloader();
		};
	}
}