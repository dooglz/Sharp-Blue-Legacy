#pragma once
#include <String>
#include <hash_map>

namespace Engine {
    // This class is responsible for loading in mesh files, and decoding them to
    // Mesh objects
    class Mesh;
    class CMeshloader {
    protected:
        std::hash_map<std::string, Mesh*> _meshLibrary;
    private:
    public:
        virtual Mesh* openOBJFile(const std::string& filename) = 0;
        virtual void loadOnGPU(Mesh* msh) = 0;
        Mesh* getMesh(std::string name);
        void Shutdown();
    };
    // global public reference to the loader
    extern CMeshloader* MeshLoader;
}