#include "Material.h"
#include "Storage.h"
#include "Shaders.h"
#include "Renderer.h"

namespace Engine{
  namespace Materials{
    Material md = { "Default", "basic", "basic", NULL, NULL, 1,0 };
    Material* mat_Default = &md;
  }


  void Material::Load(){
    if (VS == NULL){
      //load Vertex Shader
      VS = Storage<VertexShader>::Get(vsName);
    }
    if (FS == NULL){
      VS = Storage<FragmentShader>::Get(fsName);
    }
    Renderer->MakeProgram(VS,FS);
  }


}