#include "Renderer.h"

namespace Engine {
CRenderer* Renderer = NULL;

void CRenderer::SetViewMatrix(const Matrix4& vm) {
  _viewMat = vm;
  _viewprojectionMat = _projectionMat * _viewMat;
}

CRenderer::CRenderer() {
  _swapFlag = 0;
  _projectionMat = Perspective((float)(60.0f * (M_PI / 180.0f)), (16.0f / 9.0f),
                               1.0f, 2000.0f);
};
}