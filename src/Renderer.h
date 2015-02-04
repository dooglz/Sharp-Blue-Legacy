#pragma once
#include "Utilities.h" // asserts and commmon defines
#include "Maths.h"
#include <String>


namespace Engine {
struct RenderObject;
class FragmentShader;
class VertexShader;
class ShaderProgram;
class CRenderer {

protected:
  //! Number of surfaces/buffers to init.
  const static int _numberOfSurfaces = 2;

  bool _swapFlag;

  Matrix4 _viewMat;
  Matrix4 _projectionMat;
  Matrix4 _viewprojectionMat;

public:
  CRenderer();
  virtual void SetViewMatrix(const Matrix4& vm);
  virtual void RenderMesh(RenderObject* const msh, const Matrix4& mvp) = 0;
  virtual void DrawLine(const Vector3& p1, const Vector3& p2) = 0;
  virtual void DrawCross(const Vector3& p1, const float size) = 0;
  virtual void PrepFrame() = 0;
  virtual void PostRender() = 0;

  virtual void Init() = 0;

  virtual void SetClearColour(const float r, const float g, const float b) = 0;

  virtual void Shutdown(){};
  virtual ~CRenderer(){};
};

// global public reference to the renderer
extern CRenderer* Renderer;
}