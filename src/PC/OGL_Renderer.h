#pragma once
#include "../Renderer.h"
#include "../Resource.h"
#include "../Maths.h"
#include <vector>

namespace Engine {
  namespace OGL {
    class OGL_ShaderProgram;
    class COGL_Renderer : public CRenderer {
    private:
    protected:
     // static OGL::OGL_ShaderProgram* _defaultProgram;
      static void loadShaders();
      static std::vector<const Vector3> linebuffer;
      void ProcessLines();
      void ClearSurface();

    public:
      COGL_Renderer();
      void PrepFrame();
      void PostRender();

      void Init();

      void Shutdown();

      void RenderMesh(RenderObject* const msh, const Matrix4& mvp);
      void DrawLine(const Vector3& p1, const Vector3& p2);
      void DrawCross(const Vector3& p1, const float size);
      //
      static OGL::OGL_ShaderProgram* GetDefaultShaderProgram();
      RenderObject* GetNewRenderObject() ;
      ShaderProgram* MakeProgram(FragmentShader* FS, VertexShader* VS);
    };

    struct OGLRenderObject : RenderObject {
      OGL_ShaderProgram* program;
    };

  }
}
