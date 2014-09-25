#pragma once
#include "GXM_Shader.h"
#include <vector>

namespace Engine{
	namespace GXM{
		class GXM_Vertex_Shader: public GXM_Shader{

		protected:
			void patch();
			//! A map of patches for this shader
			std::vector<SceGxmVertexProgram*> _patches;
		public:
			GXM_Vertex_Shader(const std::string _name, const SceGxmProgram* pgrm)
				: GXM_Shader(_name,pgrm){};
			void createPatch(
				SceGxmVertexAttribute* VertexAttribute,
				uint8_t vertexAttributesCount,
				SceGxmVertexStream* VertexStream,
				uint8_t vertexStreamsCount
				);

				SceGxmVertexProgram* getPatchedProgram();
		};


	}
}