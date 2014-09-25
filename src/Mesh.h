#pragma once

#include "Utilities.h"		// asserts and commmon defines
#include "GMaths.h"
#if defined(_PS3_)
	#include "PS3/GCM_FragmentShader.h"
	#include "PS3/GCM_VertexShader.h"
#elif defined(_WINDOWS_)
	#include "PC/OGL_ShaderProgram.h"
#endif

#include <vector>

struct stMesh{
	bool loadedMain;
	bool loadedLocal;
	int numVerts;
	bool strip;
	bool fan;
	bool line;
	// mesh data in main memory
	std::vector<stVertex> vertexData;

	#if defined(_PS3_)
		//! A reference to the Fragment shader to use
		Engine::GCM::GCM_FragmentShader * fragShader;

		//! A reference to the Vertex shader to use
		Engine::GCM::GCM_VertexShader * vertShader;

		//Pointer to the vertex buffer in local memory
		stVertex* vertexBuffer;
		unsigned int vertexBufferOffset;

	#elif defined(_WINDOWS_)
		//! A reference to the shader program to use
		Engine::OGL::OGL_ShaderProgram * program;
		GLuint gVBO;
		GLuint gVAO;
		GLuint gIBO;
	#endif
};

static void swapMeshColorEndianmode(stMesh* m)
{
	/*CG shaders work in bigendian, which means that the stvertex.rgba
	will be read as (abgr), whereas glsl reads it as (rgba)*/
	for (int i=0; i< (m->numVerts); ++i)
	{
		unsigned int r = m->vertexData[i].rgba;
		//Unsigned 16 bit conversion:
		//m->vertexData[i].rgba = ((r>>8) | (r<<8));

		//Unsigned 32-bit conversion:
		m->vertexData[i].rgba =  ((r>>24)&0xff) |((r<<8)&0xff0000) | ((r>>8)&0xff00) | ((r<<24)&0xff000000); 

	}
}