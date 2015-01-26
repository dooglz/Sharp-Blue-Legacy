#pragma once
#include <string>


namespace Engine {
struct Mesh;
namespace SDL {

class CSDL_Meshloader {
protected:
private:
public:
  CSDL_Meshloader();
  static Mesh* openOBJFile(const std::string& filename);
  static void loadOnGPU(Mesh* msh);
  ~CSDL_Meshloader();
};
}
}