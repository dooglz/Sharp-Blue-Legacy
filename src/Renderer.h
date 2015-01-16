#pragma once
#include "Utilities.h" // asserts and commmon defines
#include "Maths.h"
#include <String>
#include "Mesh.h"

namespace Engine {
class CRenderer {

protected:
  //! Number of surfaces/buffers to init.
  const static int _numberOfSurfaces =
      2; // in this case just 2, Front and back buffers.

  //! The array of render surfaces.
  //	CellGcmSurface _surfaces[_numberOfSurfaces];

  // TODO: make this an int, to spport more than 2 buffers
  //! The current Buffer to Render into
  bool _swapFlag;

  Matrix4 _viewMat;
  Matrix4 _projectionMat;
  Matrix4 _viewprojectionMat;

public:

    virtual void SetViewMatrix(const Matrix4& vm);

  Matrix4 projMatrix;
  Matrix4 viewMatrix;

  // TODO: put these in a struct or something
  unsigned int color_pitch;
  unsigned int color_size;
  unsigned int depth_pitch;
  unsigned int depthSize;

  //! Output screen Width, in pixels
  unsigned short screenWidth;

  //! Output screen Height, in pixels
  unsigned short screenHeight;

  //! Output screen ratio, width/height
  float screenRatio;


  virtual void Init(){};

  virtual void assignShader(stMesh* m, std::string name){};

  //! Constructor, calls InitDisplay() and InitSurfaces()
  CRenderer();

  //! Detect and set resolution
  virtual void InitDisplay(){};

  //! Create Buffers/surfaces in memory
  virtual void InitSurfaces(){};

  virtual void LoadDefaultShaders(){};

  // Render loops funcs ---------

  virtual void SetupFrame(){};

  virtual void ClearSurface(){};

  //! Switch which buffer is active(being rendered on) and currentyl displayed
  virtual void SwapBuffers(){};

  //! Initialises viewport (coordinate scaling)
  virtual void SetViewport(){};

  virtual void Shutdown(){};

  virtual ~CRenderer(){};

  virtual void RenderMesh(stMesh* msh, Matrix4 mvp){};
  virtual void RenderPoly(float a1, float a2, float a3, Matrix4 mvp){};
};

// global public reference to the renderer
extern CRenderer* Renderer;
}