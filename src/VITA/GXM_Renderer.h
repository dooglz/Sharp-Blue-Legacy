#pragma once
#include "../Utilities.h"		// asserts and commmon defines
#include "../Renderer.h"		// asserts and commmon defines
#include <ctrl.h>
#include <gxm.h>
#include "GXM_Vertex_Shader.h"
#include "GXM_Fragment_Shader.h"

//During the build, compiled shader programs are inserted into these externs.
extern const SceGxmProgram _binary_clear_v_gxp_start;
extern const SceGxmProgram _binary_clear_f_gxp_start;
extern const SceGxmProgram _binary_basic_v_gxp_start;
extern const SceGxmProgram _binary_basic_f_gxp_start;

namespace Engine{
	namespace GXM{

		#define MSAA_MODE	SCE_GXM_MULTISAMPLE_NONE

		//	Define the number of back buffers to use
		#define DISPLAY_BUFFER_COUNT		3
		
		//Define the maximum number of queued swaps that the display queue will allow.
		#define DISPLAY_MAX_PENDING_SWAPS	2

		#define DISPLAY_WIDTH				960
		#define DISPLAY_HEIGHT				544
		#define DISPLAY_STRIDE_IN_PIXELS	1024
		#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
		#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8

		// Data structure for clear geometry
		typedef struct ClearVertex {
			float x;
			float y;
		} ClearVertex;

		// Data structure for basic geometry
		typedef struct BasicVertex {
			float x;
			float y;
			float z;
			uint32_t color;
		} BasicVertex;

		/*	Data structure to pass through the display queue.  This structure is
		serialized during sceGxmDisplayQueueAddEntry, and is used to pass
		arbitrary data to the display callback function, called from an internal
		thread once the back buffer is ready to be displayed.
		In this example, we only need to pass the base address of the buffer.
		*/
		typedef struct DisplayData
		{
			void *address;
		} DisplayData;

		/*	  This is purely for convenience, it is
		equivalent to simply load the binary file into memory and cast the contents
		to type SceGxmProgram.
		*/
		//CUSTOM BUILD STEP ISNT DOING THIS, FIXIT

		class GXM_Renderer : public Renderer{

		private:
			int err;
			//! A reference to the current Fragment shader in use
			static SceGxmFragmentProgram* _currentFrag;

			//! A reference to the current Vertex shader in use
			static SceGxmVertexProgram* _currentVert;

			//! Holds button information (TODO: Move to event mngr)
			SceCtrlData ctrlData;

			uint32_t backBufferIndex;
			uint32_t frontBufferIndex;
			float rotationAngle;

			//!GXM Render Context
			static SceGxmContext* _context;
			
			//!GXM Render Target
			SceGxmRenderTarget* _renderTarget;

			//! The Render surfaces
			SceGxmColorSurface _surfaces[DISPLAY_BUFFER_COUNT];
			//! The sync objects for each surface
			SceGxmSyncObject *_surfacesSync[DISPLAY_BUFFER_COUNT];
			//! Pixel data array for each surface
			void *_surfacesData[DISPLAY_BUFFER_COUNT];

			//! Depth/Stencil buffer
			SceGxmDepthStencilSurface _depthSurface;

			// Default Shader code
			static const SceGxmProgram* const clearVertexProgramGxp;
			static const SceGxmProgram* const clearFragmentProgramGxp;
			static const SceGxmProgram* const basicVertexProgramGxp;
			static const SceGxmProgram* const basicFragmentProgramGxp;

			ClearVertex* clearVertices;
			uint16_t* clearIndices;

			// Callback function for displaying a buffer
			static void displayCallback(const void *callbackData);

			//! Traingle data transform
			float wvpData[16];
			//Traingle Data
			BasicVertex * basicVertices;
			//Triangle indices
			uint16_t *basicIndices;

		public:
			//! The array of render surfaces.
			//static CellGcmSurface surfaces[_numberOfSurfaces];

			void init();

			static void sysutil_callback(uint64_t status, uint64_t param, void *userdata);

			void assignShader(stMesh* m, std::string name);

			//! Constructor, calls InitDisplay() and InitSurfaces()
			GXM_Renderer(void);

			~GXM_Renderer(){};

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
			static void SetCurrentShader(SceGxmVertexProgram & vert, SceGxmFragmentProgram & frag);

			//! Initialises viewport (coordinate scaling)
			void SetViewport();

			void shutdown(){};

			//TODO: put these in a struct or something
			uint32_t color_pitch;
			uint32_t color_size;
			uint32_t depth_pitch;
			uint32_t depthSize;

			void renderMesh(stMesh* msh, matrix4 mvp);

			GXM_Vertex_Shader* blankVertShader;
			GXM_Fragment_Shader* blankFragShader;
			GXM_Vertex_Shader* basicVertShader;
			GXM_Fragment_Shader* basicFragShader;
		};
	}
}