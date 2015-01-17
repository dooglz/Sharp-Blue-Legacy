#include "Mesh.h"

namespace Engine {

    Mesh::Mesh() {
        printf("Mesh constructor\n");
        loadedMain = false;
        loadedLocal = false;
        numVerts = 0;
        strip = false;
        fan = false;
        line = false;
        #if defined(_WINDOWS_)
        program = 0;
        #endif
    }
}