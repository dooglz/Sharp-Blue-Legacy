#pragma once
#include "OGL_Renderer.h"
#include "../Utilities.h"
#include <sdl\SDL_opengl.h>
#include <gl\glu.h>
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "../Mesh.h"

namespace Engine{
	namespace OGL{

		void SDLVersionInfo()
		{
			SDL_version compiled;
			SDL_version linked;

			SDL_VERSION(&compiled);
			SDL_GetVersion(&linked);
			printf("Compiled against SDL version %d.%d.%d ...\n",
				compiled.major, compiled.minor, compiled.patch);
			printf("linking against SDL version %d.%d.%d.\n",
				linked.major, linked.minor, linked.patch);
		}
		void GlewInfo()
		{
			printf("----------------------------------------------------------------\n");
			printf("Graphics Successfully Initialized\n");
			printf("OpenGL Info\n");
			printf("    Version: %s\n", glGetString(GL_VERSION));
			printf("     Vendor: %s\n", glGetString(GL_VENDOR));
			printf("   Renderer: %s\n", glGetString(GL_RENDERER));
			printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
			printf("----------------------------------------------------------------\n");
		}


		void CheckGL(){
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				printf("An OGL error has occured: %i\n", err);
				//DBG_HALT;
			}
		}

		void CheckSDL(){
			const char* err = SDL_GetError();
			if (strlen(err) != 0){
				printf("SDL error: %s\n", err);
				SDL_ClearError();
				//DBG_HALT;
			}
		}


		OGL_ShaderProgram* OGL_Renderer::textureProgram;
		OGL_ShaderProgram * OGL_Renderer::defaultProgram;

		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;

		OGL_Renderer::OGL_Renderer()
		{

		}

		void OGL_Renderer::init()
		{
			printf("Renderer Init \n");
			// Projection matrix : 60° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
			projMatrix = M4::perspective(60.0f, (16.0f / 9.0f), 1.0f, 2000.0f);
			InitDisplay();
			InitSurfaces();
			loadDefaultShaders();
		}

		void OGL_Renderer::InitDisplay(){
			SDLVersionInfo();
			printf("Making Display \n");
			//Initialize SDL
			ASSERT_FUNC((SDL_Init(SDL_INIT_VIDEO) >= 0), printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()));

			//Use OpenGL 3.1 core
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			//TODO: check for highest support OGL version

			//Create an ogl window
			_window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			ASSERT_FUNC((_window != NULL), printf("Window could not be created! SDL_Error: %s\n", SDL_GetError()));
		
			//Create context
			_gContext = SDL_GL_CreateContext(_window);
			ASSERT_FUNC((_gContext != NULL), printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError()));

			//Initialize GLEW
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			//Glewinit always throws an error, due to that experimental flag, just ignore it.
			glGetError();

			ASSERT_FUNC((glewError == GLEW_OK), printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError)));
			checkerr();
			GlewInfo();

			//Use Vsync
			ASSERT_FUNC((SDL_GL_SetSwapInterval(1) >= 0), printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError()));

			checkerr();

			int r = 0;
			int g = 0;
			SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &r);
			SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &g);
			printf("Major:%i, Minor:%i\n");

		};

		void OGL_Renderer::InitSurfaces(){};

		void OGL_Renderer::setupFrame(){

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glClearDepth(1.0f);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			// Cull triangles which normal is not towards the camera
			//glEnable(GL_CULL_FACE);
			//glViewport(0,0,1920,1080);

			checkerr();
		};

		void OGL_Renderer::clearSurface()
		{
			glClearColor(0.5f, 0.5f, 0.5f, 1.f);
			checkerr();

			//SDL_Delay( 16 );

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			checkerr();
		}

		void OGL_Renderer::loadDefaultShaders(){
			printf("Loading Shaders \n");
			defaultProgram = new OGL_ShaderProgram();

			//Create vertex shader
			OGL_VertexShader * VS = new OGL_VertexShader();
			VS->LoadSourceShader("shaders/basic.vert");
			checkerr();

			//Attach vertex shader to program
			defaultProgram->attachShader(VS);

			//Create Fragment shader
			OGL_FragmentShader * FS = new OGL_FragmentShader();
			FS->LoadSourceShader("shaders/basic.frag");
			checkerr();

			//Attach vertex shader to program
			defaultProgram->attachShader(FS);
			checkerr();

			//Link program
			defaultProgram->link();

			//
			textureProgram = new OGL_ShaderProgram();

			//Create vertex shader
			VS = new OGL_VertexShader();
			VS->LoadSourceShader("shaders/textured.vert");
			checkerr();

			//Attach vertex shader to program
			textureProgram->attachShader(VS);

			//Create Fragment shader
			FS = new OGL_FragmentShader();
			FS->LoadSourceShader("shaders/textured.frag");
			checkerr();

			//Attach vertex shader to program
			textureProgram->attachShader(FS);
			checkerr();

			//Link program
			textureProgram->link();

		}

		//! Switch which buffer is active(being rendered on) and currentyl displayed
		void OGL_Renderer::swapBuffers(){

			SDL_GL_SwapWindow(_window);

		}

		void OGL_Renderer::shutdown(){
			//Destroy window
			SDL_DestroyWindow(_window);

			//Quit SDL subsystems
			SDL_Quit();
		}

		//! Initialises viewport (coordinate scaling)
		void OGL_Renderer::SetViewport(){


		}

		void OGL_Renderer::checkerr(){
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				printf("An OGL error has occured: %i\n", err);
				HALT;
			}
		}

		void OGL_Renderer::assignShader(stMesh* m, std::string name)
		{
			//TODO: Shader store
			m->program = defaultProgram;
		};

		void OGL_Renderer::renderMesh(stMesh* msh, Matrix4 mvp)
		{
			glUseProgram(msh->program->getID());
			OGL_Renderer::checkerr();

			//get shader input indexes
			GLint mvpIn = glGetUniformLocation(msh->program->getID(), "MVP");
			OGL_Renderer::checkerr();

			//Send MVP
			glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(mvp));
			OGL_Renderer::checkerr();

			//Bind to VAO
			glBindVertexArray(msh->gVAO);
			OGL_Renderer::checkerr();

			//DRAW
			if (msh->line == true){
				if (msh->strip == true){
					glDrawArrays(GL_LINE_STRIP, 0, msh->numVerts);
				}
				else{
					glDrawArrays(GL_LINES, 0, msh->numVerts);
				}
			}
			else{
				if (msh->fan == true){
					glDrawArrays(GL_TRIANGLE_FAN, 0, msh->numVerts);
				}
				else if (msh->strip == true){
					glDrawArrays(GL_TRIANGLE_STRIP, 0, msh->numVerts);
				}
				else{
					glDrawArrays(GL_TRIANGLES, 0, msh->numVerts);
				}
			}
			OGL_Renderer::checkerr();

			//unbind VAO
			glBindVertexArray(0);
			OGL_Renderer::checkerr();

			//Unbind program
			glUseProgram(NULL);
			OGL_Renderer::checkerr();
		};

		void OGL_Renderer::renderPoly(float a1, float a2, float a3, Matrix4 mvp)
		{
			//OGL_Meshloader::loadOnGPU(stMesh* msh)
		};
	}
}