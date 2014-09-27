#pragma once
#include "Mesh.h"
#include "GMaths.h"
namespace Engine{
	//This class is responsible for loading in mesh files, and decoding them to Mesh objects
	class Meshloader{
	protected:
		virtual bool loadOBJ(const char * path, std::vector<stvec3> & out_vertices, std::vector<stvec2> & out_uvs, std::vector<stvec3> & out_normals){ return false; };
	private:

	public:
		//blank constuctor
		Meshloader(){};
		virtual stMesh* loadOBJFile(const std::string &filename) { return NULL; };
		virtual void loadOnGPU(stMesh* msh){};
		virtual void render(stMesh* msh, Matrix4 mvp) {};
		virtual ~Meshloader(){};
	};
}