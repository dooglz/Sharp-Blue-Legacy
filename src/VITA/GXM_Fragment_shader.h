#pragma once
#include "GXM_Shader.h"
#include <vector>

namespace Engine{
	namespace GXM{
		class GXM_Fragment_Shader: public GXM_Shader{

		protected:
			void patch();
			//! A map of patches for this shader
			std::vector<SceGxmFragmentProgram*> _patches;
		public:
			GXM_Fragment_Shader(const std::string _name, const SceGxmProgram* pgrm)
				: GXM_Shader(_name,pgrm){};
			void createPatch(
				    const SceGxmOutputRegisterFormat outputFormat,
					const SceGxmMultisampleMode multisampleMode,
					const SceGxmBlendInfo *blendInfo,
					const SceGxmProgram *vertexProgram
				);
			SceGxmFragmentProgram* getPatchedProgram();
		};

	}
}