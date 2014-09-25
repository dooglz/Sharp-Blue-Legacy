#pragma once
#include "../Meshloader.h"
#include "../Utilities.h"		// asserts and commmon defines
namespace Engine{
	namespace OGL{
		//This clas sis responsible for loading in mesh files, and decoding them to Mesh objects
		class OGL_Meshloader : public Meshloader{
		protected:
			bool loadOBJ(const char * path, std::vector<stvec3> & out_vertices, std::vector<stvec2> & out_uvs, std::vector<stvec3> & out_normals);
		private:

		public:
			//blank constuctor
			OGL_Meshloader(){};
			stMesh* loadOBJFile(const std::string &filename);
			void loadOnGPU(stMesh* msh);
		};
	}
}