#include "MeshLoader.h"
#include <iostream>
#include <string>

namespace Engine {
	Mesh* CMeshloader::getMesh(std::string name) {
		// printf("looking for mesh ! : %s\n", name.c_str());
		// lookup
		std::hash_map<std::string, Mesh*>::iterator got = _meshLibrary.find(name);
		if (got == _meshLibrary.end()) {
			// Not loaded
			//	printf("Mesh not loaded ! : %s\n", name.c_str());
			Mesh* _mesh = MeshLoader->openOBJFile(name);
			MeshLoader->loadOnGPU(_mesh);
			_meshLibrary.insert(std::make_pair(name, _mesh));
			return _mesh;
		}
		else {
			return got->second;
		}
	}
	CMeshloader* MeshLoader = 0;
}