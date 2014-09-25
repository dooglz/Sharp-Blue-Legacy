#pragma once
#include "../Meshloader.h"
#include "../Utilities.h"		// asserts and commmon defines
namespace Engine{
	namespace GXM{
		//This clas sis responsible for loading in mesh files, and decoding them to Mesh objects
		class GXM_Meshloader : public Meshloader{
		protected:
			bool loadOBJ(const char * path, std::vector<stvec3> & out_vertices, std::vector<stvec2> & out_uvs, std::vector<stvec3> & out_normals){return true;};
		private:

		public:
			//blank constuctor
			GXM_Meshloader(){};
			stMesh* loadOBJFile(const std::string &filename){return NULL;};
			void loadOnGPU(stMesh* msh){};
		};
	}
}