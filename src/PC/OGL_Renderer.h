#pragma once
#include "sdl/SDL.h"
#include "glew/glew.h"
#include "../Utilities.h"		// asserts and commmon defines
#include "../Renderer.h"		// asserts and commmon defines
#include "OGL_FragmentShader.h"
#include "OGL_VertexShader.h"
#include "OGL_ShaderProgram.h"

namespace Engine{
	namespace OGL{
		class OGL_Renderer : public Renderer{

		private:
			//! A reference to the current Fragment shader in use
			//OGL_FragmentShader * _currentFrag;

			//! A reference to the current Vertex shader in use
			//OGL_VertexShader * _currentVert;

			//! A reference to the default Fragment shader
			OGL_FragmentShader * defaultFrag;

			//! A reference to the default Vertex shader
			OGL_VertexShader * defaultVert;

			static OGL_ShaderProgram * defaultProgram;

			static OGL_ShaderProgram * textureProgram;

			SDL_GLContext _gContext;

			//The window we'll be rendering to
			SDL_Window* _window;

		public:
			void init();

			static void checkerr();

			void assignShader(stMesh* m, std::string name);

			//! Constructor, calls InitDisplay() and InitSurfaces()
			OGL_Renderer();

			//! Detect and set resolution
			void InitDisplay();

			//! Create Buffers/surfaces in memory
			void InitSurfaces();

			void setupFrame();

			void clearSurface();

			void loadDefaultShaders();

			//! Switch which buffer is active(being rendered on) and currentyl displayed
			void swapBuffers();

			//! Set the active vertex and fragment shader
			void SetCurrentShader(OGL_VertexShader & vert, OGL_FragmentShader & frag);

			//! Initialises viewport (coordinate scaling)
			void SetViewport();

			void shutdown();

			//TODO: put these in a struct or something
			unsigned int color_pitch;
			unsigned int color_size;
			unsigned int depth_pitch;
			unsigned int depthSize;

			void renderMesh(stMesh* msh, matrix4 mvp);

			void renderPoly(float a1, float a2, float a3, matrix4 mvp);

			static OGL_ShaderProgram * getTextureProgram(){ return textureProgram; }
			static OGL_ShaderProgram * getDefaultProgram(){ return defaultProgram; }
		};
	}
}