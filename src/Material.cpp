#include "Material.h"
#include "Storage.h"
#include "Shaders.h"
#include "Renderer.h"

namespace Engine{
  namespace Materials{
    Material md = { "Default", "basic", "basic", NULL, NULL, NULL, 1, 0 };
    Material* mat_Default = &md;
  }


  void Material::Load(){
    if (VS == NULL){
      //load Vertex Shader
      VS = Storage<VertexShader>::Get(vsName);
    }
    if (FS == NULL){
      FS = Storage<FragmentShader>::Get(fsName);
    }
    //TODO: mkae program a get from resource
    printf("Linking shader - %s with %s\n", vsName.c_str(),fsName.c_str());
    program = Renderer->MakeProgram(FS,VS);

  }


}