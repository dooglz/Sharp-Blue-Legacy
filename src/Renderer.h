#pragma once
#include "Utilities.h"		// asserts and commmon defines
#include "GMaths.h"
#include <String>
#include "Mesh.h"

namespace Engine{
	class Renderer{

	protected:

		//! Number of surfaces/buffers to init.
		const static int _numberOfSurfaces = 2;	//in this case just 2, Front and back buffers.

		//! The array of render surfaces.
		//	CellGcmSurface _surfaces[_numberOfSurfaces];

		//TODO: make this an int, to spport more than 2 buffers
		//! The current Buffer to Render into
		bool _swapFlag;

	public:
		matrix4 projMatrix;

		virtual void init(){};

		virtual void assignShader(stMesh* m, std::string name){};

		//! Constructor, calls InitDisplay() and InitSurfaces()
		Renderer(){};

		//! Output screen Width, in pixels
		unsigned short screenWidth;

		//! Output screen Height, in pixels
		unsigned short screenHeight;

		//! Output screen ratio, width/height
		float screenRatio;

		//! Detect and set resolution
		virtual void InitDisplay(){};

		//! Create Buffers/surfaces in memory
		virtual void InitSurfaces(){};

		virtual void loadDefaultShaders(){};

		//Render loops funcs ---------

		virtual void setupFrame(){};

		virtual void clearSurface(){};

		//! Switch which buffer is active(being rendered on) and currentyl displayed
		virtual void swapBuffers(){};

		//! Initialises viewport (coordinate scaling)
		virtual void SetViewport(){};

		virtual void shutdown(){};

		//TODO: put these in a struct or something
		unsigned int color_pitch;
		unsigned int color_size;
		unsigned int depth_pitch;
		unsigned int depthSize;

		virtual ~Renderer(){};

		virtual void renderMesh(stMesh* msh, matrix4 mvp){};
		virtual void renderPoly(float a1, float a2, float a3, matrix4 mvp){};
	};
}