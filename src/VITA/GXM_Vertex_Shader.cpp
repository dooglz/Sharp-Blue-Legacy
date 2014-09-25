#include "GXM_Vertex_Shader.h"

namespace Engine{
	namespace GXM{

		void GXM_Vertex_Shader::createPatch( SceGxmVertexAttribute* vertexAttribute,
				uint8_t vertexAttributesCount, SceGxmVertexStream* vertexStream,
				uint8_t vertexStreamsCount)
		{
			printf("Shader %s, Patching with %i attributes and %i streams \n",_name.c_str(),vertexAttributesCount,vertexStreamsCount);
			SceGxmVertexProgram* patchedProgram;
			int err = sceGxmShaderPatcherCreateVertexProgram(
				shaderPatcher,
				_patcherID,
				vertexAttribute,
				vertexAttributesCount,
				vertexStream,
				vertexStreamsCount,
				&patchedProgram);
			SCE_DBG_ASSERT(err == SCE_OK);
			_patches.push_back(patchedProgram);
		}

		
		SceGxmVertexProgram* GXM_Vertex_Shader::getPatchedProgram()
		{
			return _patches.front();
		}
}}