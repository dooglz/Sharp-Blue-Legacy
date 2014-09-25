#pragma once
#define COMMAND_SIZE (65536) // 64 KB
#define BUFFER_SIZE (1024*1024) //1MB

#include "Utilities.h"		// asserts and commmon defines
#include "Renderer.h"		// asserts and commmon defines
#include "GCM_FragmentShader.h"
#include "GCM_VertexShader.h"
#include <cell/gcm.h>	//for CGprogram
namespace Engine{
	namespace GCM{
		class GCM_Renderer : public Renderer{

		private:

			//! A reference to the current Fragment shader in use
			static GCM_FragmentShader * _currentFrag;

			//! A reference to the current Vertex shader in use
			static GCM_VertexShader * _currentVert;

			//! A reference to the default Fragment shader
			static GCM_FragmentShader * defaultFrag;

			//! A reference to the default Vertex shader
			static GCM_VertexShader * defaultVert;

		public:
			//! The array of render surfaces.
			static CellGcmSurface _surfaces[_numberOfSurfaces];


			void init();

			static void sysutil_callback(uint64_t status, uint64_t param, void *userdata);

			void assignShader(stMesh* m, std::string name);

			//! Constructor, calls InitDisplay() and InitSurfaces()
			GCM_Renderer(void);

			~GCM_Renderer(){};

			//! Output screen ratio, width/height
			float screenRatio;

			//! Detect and set resolution
			void InitDisplay();

			//! Create Buffers/surfaces in memory
			void InitSurfaces();

			void loadDefaultShaders();

			void setupFrame();

			void clearSurface();

			//! Switch which buffer is active(being rendered on) and currentyl displayed
			void swapBuffers();

			//! Set the active vertex and fragment shader
			static void SetCurrentShader(GCM_VertexShader & vert, GCM_FragmentShader & frag);

			//! Initialises viewport (coordinate scaling)
			void SetViewport();

			void shutdown(){};

			//TODO: put these in a struct or something
			uint32_t color_pitch;
			uint32_t color_size;
			uint32_t depth_pitch;
			uint32_t depthSize;

			void renderMesh(stMesh* msh, matrix4 mvp);
		};
	}
}