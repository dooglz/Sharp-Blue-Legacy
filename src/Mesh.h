#pragma once

#include "Utilities.h"		// asserts and commmon defines
#include "GMaths.h"
#include <vector>

#if defined(_PS3_)
	namespace Engine{
		namespace GCM{
			class GCM_FragmentShader;
			class GCM_VertexShader;
		};
	};
#elif defined(_WINDOWS_)
	namespace Engine{
		namespace OGL{
			class OGL_ShaderProgram;
		};
	};
#endif

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
		unsigned int gVBO;
		unsigned int gVAO;
		unsigned int gIBO;
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