#pragma once
#include "Utilities.h" // asserts and commmon defines
#include "Maths.h"
#include <String>
#include "Mesh.h"

namespace Engine {
class Mesh;
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
  virtual void RenderMesh(Mesh* const msh, const Matrix4& mvp) = 0;
  virtual void DrawLine(const Vector3& p1, const Vector3& p2) = 0;
  virtual void DrawCross(const Vector3& p1, const float size) = 0;
  virtual void PrepFrame() = 0;
  virtual void PostRender() = 0;

  virtual void Init() = 0;

  virtual void Shutdown(){};
  virtual ~CRenderer(){};

  virtual void RenderPoly(float a1, float a2, float a3, Matrix4 mvp){};
};

// global public reference to the renderer
extern CRenderer* Renderer;
}